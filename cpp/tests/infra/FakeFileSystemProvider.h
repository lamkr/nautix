#pragma once

#include "application/filesystem_provider.h"
#include "application/directory_metadata.h"
#include <unordered_map>

namespace app = nautix::application;

class FakeFileSystemProvider final : public app::IFileSystemProvider {
public:
    std::expected<bool, std::error_code> create_directory(const app::DirectoryMetadata& metadata) override;

    [[nodiscard]] std::expected<app::DirectoryMetadata, std::error_code>
        get_metadata(const char* path) const override;

private:
    std::unordered_map<const char*, app::DirectoryMetadata> metadatas;
};
