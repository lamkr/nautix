#pragma once

#include "../application/directory_metadata_provider.h"

namespace nautix::infra {

    class SystemMetadataProvider final : public application::IDirectoryMetadataProvider
    {
    public:
        std::expected<application::DirectoryMetadata, std::error_code> get_metadata(const char* path) const override;
    };

}
