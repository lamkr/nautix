#pragma once

#include <expected>
#include <system_error>

#include "directory_metadata.h"

namespace nautix::application {

    class IFileSystemProvider {
    public:
        virtual ~IFileSystemProvider() = default;

        virtual std::expected<bool, std::error_code> create_directory(const DirectoryMetadata& metadata) = 0;

        [[nodiscard]] virtual std::expected<DirectoryMetadata, std::error_code> get_metadata(const char* path) const = 0;
    };

}
