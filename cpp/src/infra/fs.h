#pragma once

#include <chrono>
#include <sys/stat.h>
#include <expected>
#include <filesystem>
#include <system_error>
#include "../domain/owner.h"
#include "../domain/local_time.h"

//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    get_creation_time(const struct stat& stat);
//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    get_modification_time(const struct stat& stat);
//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    get_access_time(const struct stat& stat);
//std::chrono::time_point<std::chrono::local_t, std::chrono::duration<long, std::nano>>
//std::chrono::local_time<std::chrono::duration<long, std::nano>>
nautix::domain::LocalTime
    to_local_time(std::chrono::system_clock::time_point time_point);

nautix::domain::Owner get_owner_from_id(const struct stat& stat);

std::expected<std::string, std::error_code> get_owner_name(uid_t owner_id);

std::expected<const char*, std::error_code> get_home_path() noexcept;

std::uintmax_t compute_directory_size(const std::filesystem::path& path);
