#pragma once

#include <chrono>
#include <expected>
#include <filesystem>
#include <sys/stat.h>

#include "directory.h"

namespace fs = std::filesystem;
namespace chrono = std::chrono;
namespace domain = nautix::domain;

chrono::time_point<chrono::local_t, chrono::duration<long, std::nano>> get_creation_time(
    const struct stat& stat);
chrono::time_point<chrono::local_t, chrono::duration<long, std::nano>> get_modification_time(
    const struct stat& stat);
chrono::time_point<chrono::local_t, chrono::duration<long, std::nano>> get_access_time(
    const struct stat& stat);
std::string get_owner_name(const struct stat& stat);

chrono::system_clock::time_point get_creation_time_legacy(const fs::path& path);
chrono::system_clock::time_point get_creation_time(const fs::path& path);
chrono::system_clock::time_point get_modification_time(const fs::path&);
chrono::system_clock::time_point get_access_time(const fs::path& path);

uid_t get_owner_id(const fs::path& path);
std::expected<std::string, std::error_code> get_owner_name(uid_t owner_id);

std::expected<const char*, std::error_code> get_home_path() noexcept;

std::uintmax_t compute_directory_size(const fs::path& path);

chrono::time_point<chrono::local_t, chrono::duration<long, std::nano>> to_local_time(
    chrono::system_clock::time_point time_point);


// --------------------------------

void showInfo(const fs::path& path, int what);

void show_dirs(const std::vector<domain::Directory>& vector, int what);

void show_entries(const std::vector<fs::directory_entry>& vector, int what);
