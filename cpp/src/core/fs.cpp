#include <sys/syscall.h> // For syscall
#include <fcntl.h>       // For AT_FDCWD
#include <unistd.h>
#include <iostream>
#include "fs.h"

#include <expected>
#include <pwd.h>

#include "errors.h"

const auto UNKNOWN_OWNER_NAME = "<unknown>";

chrono::time_point<chrono::local_t, chrono::duration<long, std::nano>> get_creation_time(
    const struct stat& stat) {
    const chrono::system_clock::time_point time = chrono::system_clock::from_time_t(stat.st_ctim.tv_sec) +
        chrono::nanoseconds(stat.st_ctim.tv_nsec);
    return to_local_time( time );
}

chrono::time_point<chrono::local_t, chrono::duration<long, std::nano>> get_modification_time(
    const struct stat& stat) {
    const chrono::system_clock::time_point time = chrono::system_clock::from_time_t(stat.st_mtim.tv_sec) +
        chrono::nanoseconds(stat.st_mtim.tv_nsec);
    return to_local_time( time );
}

chrono::time_point<chrono::local_t, chrono::duration<long, std::nano>> get_access_time(
    const struct stat& stat) {
    const chrono::system_clock::time_point time = chrono::system_clock::from_time_t(stat.st_atim.tv_sec) +
        chrono::nanoseconds(stat.st_atim.tv_nsec);
    return to_local_time( time );
}

std::string get_owner_name(const struct stat& stat) {
    if (const passwd* pw = getpwuid(stat.st_uid); pw != nullptr) {
        return pw->pw_name;
    }
    return UNKNOWN_OWNER_NAME;
}

chrono::system_clock::time_point get_creation_time_legacy(const fs::path& path) {
    struct stat stat_dir{};
    if (stat(path.c_str(), &stat_dir) == 0) {
        return chrono::system_clock::from_time_t(stat_dir.st_ctim.tv_sec) +
            chrono::nanoseconds(stat_dir.st_ctim.tv_nsec);
    }
    return get_modification_time(path);
}

chrono::system_clock::time_point get_creation_time(const std::filesystem::path& path) {
    struct statx stat{};
    constexpr int descriptor = AT_FDCWD;
    constexpr unsigned int flags = AT_SYMLINK_NOFOLLOW; // Don't follow symlinks
    constexpr unsigned int mask = STATX_BTIME; // Request birth time

    if (syscall(__NR_statx, descriptor, path.c_str(), flags, mask, &stat) == 0) {
        // Convert timespec to chrono::system_clock::time_point
        return chrono::system_clock::from_time_t(stat.stx_btime.tv_sec) +
            chrono::nanoseconds(stat.stx_btime.tv_nsec);
    }
    return get_creation_time_legacy(path);
}

chrono::system_clock::time_point get_modification_time(const fs::path& path) {
    return chrono::clock_cast<chrono::system_clock>(
        std::filesystem::last_write_time(path));
}

chrono::system_clock::time_point get_access_time(const fs::path& path) {
    struct stat stat_dir{};
    if (stat(path.c_str(), &stat_dir) == 0) {
        return chrono::system_clock::from_time_t(stat_dir.st_atim.tv_sec) +
            chrono::nanoseconds(stat_dir.st_atim.tv_nsec);
    }
    return chrono::system_clock::from_time_t(time_t{});
}

uid_t get_owner_id(const fs::path& path) {
    struct stat stat_dir{};
    if (stat(path.c_str(), &stat_dir) == 0) {
        return stat_dir.st_uid;
    }
    return 0;
}

std::expected<std::string, std::error_code> get_owner_name(uid_t owner_id) {
    errno = 0;
    if (const passwd* pw = getpwuid(owner_id); pw != nullptr) {
        return pw->pw_name;
    }
    if (errno != 0) {
        return std::unexpected(std::error_code(errno, std::generic_category()));
    }
    return std::unexpected(nautix_error::user_not_found);
}

std::expected<const char*, std::error_code> get_home_path() noexcept {
    if (const char* home_path = std::getenv("HOME")) {
        return home_path;
    }
    // Fallback to POSIX systems.
    if (const passwd* pw = getpwuid(getuid())) {
        return pw->pw_dir;
    }
    return std::unexpected(std::make_error_code(std::errc::function_not_supported));
}

std::uintmax_t compute_directory_size(const fs::path& path) {
    std::error_code error;
    const std::uintmax_t size = fs::file_size(path, error);
    return !error ? size : 0; // TODO compute dir size;
}

chrono::time_point<chrono::local_t, chrono::duration<long, std::nano>> to_local_time(
    chrono::system_clock::time_point time_point) {
    // Get the local time zone.
    const std::chrono::time_zone* local_tz = std::chrono::current_zone();
    // Create a zoned_time object.
    // This object associates the utc_time with the local time zone.
    const chrono::zoned_time zt{local_tz, time_point};
    // Access the local_time from the zoned_time
    // The local_time is a time_point in the local_t type,
    // representing the time in the specified time zone.
    return zt.get_local_time();
}

//------------------------------------------

std::string getTimeISO8601(chrono::system_clock::time_point time_point) {
    // Get milliseconds
    const auto ms = chrono::duration_cast<chrono::milliseconds>(
        time_point.time_since_epoch()
    ) % 1000;

    // Convert to time_t for gmtime
    const std::time_t the_time_t = chrono::system_clock::to_time_t(time_point);
    const std::tm* tm_gmt = std::gmtime(&the_time_t); // Get UTC time

    if (tm_gmt == nullptr) {
        return ""; // Error occurred with gmtime
    }

    // Format the time
    std::ostringstream oss;
    oss << std::put_time(tm_gmt, "%Y-%m-%dT%H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count() << "Z"; // Add milliseconds and 'Z' for UTC

    return oss.str();
}

void showInfo(const fs::path& path, const int what) {
    const chrono::system_clock::time_point creationTime = get_creation_time(path);
    const chrono::system_clock::time_point modificationTime = get_modification_time(path);
    const chrono::system_clock::time_point accessTime = get_access_time(path);

    const uid_t owner = get_owner_id(path);

    chrono::system_clock::time_point theTime;
    if (what == 'C') {
        theTime = creationTime;
    }
    else if (what == 'M') {
        theTime = modificationTime;
    }
    else if (what == 'A') {
        theTime = accessTime;
    }
    std::cout << "\t" << path
        << " " << static_cast<char>(what) << ": " << getTimeISO8601(theTime)
        << " O:" << owner
        << std::endl;
}

void show_dirs(const std::vector<domain::Directory>& vector, int what) {
    std::cout << "result: " << std::endl;
    for (const auto& dir : vector) {
        showInfo(dir.path(), what);
    }
}

void show_entries(const std::vector<fs::directory_entry>& vector, int what) {
    std::cout << "dirs: " << std::endl;
    for (const auto& dir : vector) {
        showInfo(dir.path(), what);
    }
}
