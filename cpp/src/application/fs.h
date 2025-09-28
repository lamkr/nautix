#pragma once

#include <chrono>
#include <filesystem>
#include <sys/stat.h>
#include "directory.h"

namespace fs = std::filesystem;
namespace chrono = std::chrono;

chrono::system_clock::time_point get_modification_time(const fs::path&);

chrono::system_clock::time_point get_creation_time_legacy(const fs::path& path);

chrono::system_clock::time_point get_creation_time(const std::filesystem::path& path);

chrono::system_clock::time_point get_modification_time(const fs::path& path);

chrono::system_clock::time_point get_access_time(const fs::path& path);

__uid_t get_owner(const fs::path& path);

chrono::local_time<std::chrono::system_clock::duration> to_local_time(chrono::system_clock::time_point time_point);

// --------------------------------

void showInfo(const fs::path& path, int what);

void show(std::vector<nautix::domain::Directory> vect, int what);

void show(std::vector<fs::directory_entry> vect, int what);
