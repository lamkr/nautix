#include <unistd.h>
#include <pwd.h>
#include "infra/fs.h"
#include "infra/time.h"
#include "infra/errors.h"
#include "domain/local_time.h"
#include "domain/owner.h"

nautix::domain::LocalTime
    get_creation_time(const struct stat& stat)
{
    const std::chrono::system_clock::time_point time = std::chrono::system_clock::from_time_t(stat.st_ctim.tv_sec) +
        std::chrono::nanoseconds(stat.st_ctim.tv_nsec);
    return to_local_time( time );
}

nautix::domain::LocalTime
    get_modification_time(const struct stat& stat)
{
    const std::chrono::system_clock::time_point time = std::chrono::system_clock::from_time_t(stat.st_mtim.tv_sec) +
        std::chrono::nanoseconds(stat.st_mtim.tv_nsec);
    return to_local_time( time );
}

nautix::domain::LocalTime
    get_access_time(const struct stat& stat)
{
    const std::chrono::system_clock::time_point time = std::chrono::system_clock::from_time_t(stat.st_atim.tv_sec) +
        std::chrono::nanoseconds(stat.st_atim.tv_nsec);
    return to_local_time( time );
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
        get_owner_name(stat.st_uid)
        .value_or(nautix::domain::UNKNOWN) );
}