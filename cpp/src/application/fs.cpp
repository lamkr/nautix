#include <sys/syscall.h> // For syscall
#include <fcntl.h>       // For AT_FDCWD
#include <unistd.h>
#include <iostream>
#include "fs.h"

// Define __NR_statx if not already defined (might be needed for older headers)
#ifndef __NR_statx
#if defined(__x86_64__)
#define __NR_statx 332
#elif defined(__i386__)
#define __NR_statx 383
#elif defined(__aarch64__)
#define __NR_statx 291
#else
#error "Unsupported architecture for statx system call"
#endif
#endif

chrono::system_clock::time_point get_creation_time_legacy(const fs::path& path) {
    struct stat stat_dir;
    if (stat(path.c_str(), &stat_dir) == 0) {
        return chrono::system_clock::from_time_t(stat_dir.st_ctim.tv_sec) +
            chrono::nanoseconds(stat_dir.st_ctim.tv_nsec);
    }
    return get_modification_time(path);
}

// Function to get the creation time of a directory using statx
chrono::system_clock::time_point get_creation_time(const std::filesystem::path& path) {
    struct statx stat_dir;
    const int descriptor = AT_FDCWD;
    const unsigned int flags = AT_SYMLINK_NOFOLLOW; // Don't follow symlinks
    const unsigned int mask = STATX_BTIME; // Request birth time

    if (syscall(__NR_statx, descriptor, path.c_str(), flags, mask, &stat_dir) == 0) {
        // Convert timespec to chrono::system_clock::time_point
        return chrono::system_clock::from_time_t(stat_dir.stx_btime.tv_sec) +
            chrono::nanoseconds(stat_dir.stx_btime.tv_nsec);
    }
    return get_creation_time_legacy(path);
}

chrono::system_clock::time_point get_modification_time(const fs::path& path) {
    return chrono::clock_cast<chrono::system_clock>(
        std::filesystem::last_write_time(path));
}

chrono::system_clock::time_point get_access_time(const fs::path& path) {
    struct stat stat_dir;
    if (stat(path.c_str(), &stat_dir) == 0) {
        return chrono::system_clock::from_time_t(stat_dir.st_atim.tv_sec) +
            chrono::nanoseconds(stat_dir.st_atim.tv_nsec);
    }
    return chrono::system_clock::from_time_t(time_t{});
}

__uid_t get_owner(const fs::path& path) {
    struct stat stat_dir;
    if (stat(path.c_str(), &stat_dir) == 0) {
        return stat_dir.st_uid;
    }
    return 0;
}

chrono::local_time<std::chrono::system_clock::duration> to_local_time(chrono::system_clock::time_point time_point) {
    // Get the local time zone.
    const std::chrono::time_zone* local_tz = std::chrono::current_zone();
    // Create a zoned_time object.
    // This object associates the utc_time with the local time zone.
    chrono::zoned_time zt{local_tz, time_point};
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

void showInfo(const fs::path& path, int what) {
    chrono::system_clock::time_point creationTime = get_creation_time(path);
    chrono::system_clock::time_point modificationTime = get_modification_time(path);
    chrono::system_clock::time_point accessTime = get_access_time(path);

    __uid_t owner = get_owner(path);

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

void show(std::vector<nautix::domain::Directory> vect, int what) {
    std::cout << "result: " << std::endl;
    for (unsigned long x = 0; x < vect.size(); ++x) {
        auto dir = vect[x];
        showInfo(dir.path(), what);
    }
}

void show(std::vector<fs::directory_entry> vect, int what) {
    std::cout << "dirs: " << std::endl;
    for (unsigned long x = 0; x < vect.size(); ++x) {
        auto dir = vect[x];
        showInfo(dir.path(), what);
    }
}
