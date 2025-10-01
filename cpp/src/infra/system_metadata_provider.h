#pragma once

#include "../application/directory_metadata.h"
#include "../application/filesystem_provider.h"

namespace nautix::infra {

    class SystemMetadataProvider final : public application::IFileSystemProvider
    {
    public:
        std::expected<bool, std::error_code> create_directory(const application::DirectoryMetadata& metadata) override;
        std::expected<application::DirectoryMetadata, std::error_code> get_metadata(const char* path) const override;
    };

}
