#pragma once

#include <expected>
#include <chrono>
#include <string>
#include <filesystem>

namespace nautix::application {
    namespace fs = std::filesystem;
    namespace chrono = std::chrono;

    struct DirectoryMetadata {
        fs::path path;
        std::string name;
        std::uintmax_t size{};
        uid_t owner_id{};
        chrono::local_time<std::chrono::system_clock::duration> modification_time;
        chrono::local_time<std::chrono::system_clock::duration> creation_time;
        chrono::local_time<std::chrono::system_clock::duration> access_time;
    };

    class IDirectoryMetadataProvider {
    public:
        virtual ~IDirectoryMetadataProvider() = default;

        virtual std::expected<DirectoryMetadata, std::error_code> get_metadata(const char* path) const = 0;
    };

}