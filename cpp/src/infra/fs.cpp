#include <unistd.h>
#include <pwd.h>
#include "fs.h"
#include "errors.h"

const auto UNKNOWN_OWNER_NAME = "<unknown>";

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    get_creation_time(const struct stat& stat)
{
    const std::chrono::system_clock::time_point time = std::chrono::system_clock::from_time_t(stat.st_ctim.tv_sec) +
        std::chrono::nanoseconds(stat.st_ctim.tv_nsec);
    return to_local_time( time );
}

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    get_modification_time(const struct stat& stat)
{
    const std::chrono::system_clock::time_point time = std::chrono::system_clock::from_time_t(stat.st_mtim.tv_sec) +
        std::chrono::nanoseconds(stat.st_mtim.tv_nsec);
    return to_local_time( time );
}

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    get_access_time(const struct stat& stat)
{
    const std::chrono::system_clock::time_point time = std::chrono::system_clock::from_time_t(stat.st_atim.tv_sec) +
        std::chrono::nanoseconds(stat.st_atim.tv_nsec);
    return to_local_time( time );
}

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    to_local_time(std::chrono::system_clock::time_point time_point)
{
    // Get the local time zone.
    const std::chrono::time_zone* local_tz = std::chrono::current_zone();
    // Create a zoned_time object.
    // This object associates the utc_time with the local time zone.
    const std::chrono::zoned_time zt{local_tz, time_point};
    // Access the local_time from the zoned_time
    // The local_time is a time_point in the local_t type,
    // representing the time in the specified time zone.
    std::chrono::local_time<std::chrono::duration<long, std::nano>> local_tm = zt.get_local_time();
    return nautix::domain::LocalTime {local_tm};
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

std::uintmax_t compute_directory_size(const std::filesystem::path& path) {
    std::error_code error;
    const std::uintmax_t size = std::filesystem::file_size(path, error);
    return !error ? size : 0; // TODO compute dir size;
}

nautix::domain::Owner get_owner_from_id(const struct stat& stat) {
    return nautix::domain::Owner(
        stat.st_uid,
        get_owner_name(stat.st_uid).value_or(UNKNOWN_OWNER_NAME) );
}