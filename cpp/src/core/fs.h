#pragma once

#include <chrono>
#include <filesystem>
#include <sys/stat.h>

#include "directory.h"
#include "../domain/directory.h"

namespace fs = std::filesystem;
namespace chrono = std::chrono;
namespace domain = nautix::domain;

chrono::time_point<chrono::local_t, chrono::duration<long, std::ratio<1, 1000000000>>> get_creation_time(
    const struct stat& stat);
chrono::time_point<chrono::local_t, chrono::duration<long, std::ratio<1, 1000000000>>> get_modification_time(
    const struct stat& stat);
chrono::time_point<chrono::local_t, chrono::duration<long, std::ratio<1, 1000000000>>> get_access_time(
    const struct stat& stat);
std::string get_owner_name(const struct stat& stat);

chrono::system_clock::time_point get_creation_time_legacy(const fs::path& path);
chrono::system_clock::time_point get_creation_time(const fs::path& path);
chrono::system_clock::time_point get_modification_time(const fs::path&);
chrono::system_clock::time_point get_access_time(const fs::path& path);

__uid_t get_owner_id(const fs::path& path);
std::string get_owner_name(__uid_t owner_id);

std::optional<fs::path> get_home_path() noexcept;

std::uintmax_t compute_directory_size(const fs::path& path);

chrono::time_point<chrono::local_t, chrono::duration<long, std::ratio<1, 1000000000>>> to_local_time(
    chrono::system_clock::time_point time_point);

// --------------------------------

void showInfo(const fs::path& path, int what);

void show_dirs(std::vector<domain::Directory>& vect, int what);

void show_entries(std::vector<fs::directory_entry>& vect, int what);
