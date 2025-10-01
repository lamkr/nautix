#pragma once

#include <chrono>
#include <string>
#include <filesystem>

namespace nautix::application {

    namespace fs = std::filesystem;
    namespace chrono = std::chrono;

    struct DirectoryMetadata {
        fs::path path;
        std::string name;
        std::uintmax_t size{0};
        uid_t owner_id{static_cast<uid_t>(-1)};
        chrono::local_time<std::chrono::system_clock::duration> modification_time;
        chrono::local_time<std::chrono::system_clock::duration> creation_time;
        chrono::local_time<std::chrono::system_clock::duration> access_time;
    };

}