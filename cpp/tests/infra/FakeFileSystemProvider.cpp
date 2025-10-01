#include "FakeFileSystemProvider.h"

#include "core/errors.h"

std::expected<bool, std::error_code>
FakeFileSystemProvider::create_directory(const app::DirectoryMetadata& metadata) {
    if (metadatas.contains(metadata.path.c_str())) {
        return false;
    }
    metadatas[metadata.path.c_str()] = metadata;
    return true;
}

[[nodiscard]] std::expected<app::DirectoryMetadata, std::error_code>
FakeFileSystemProvider::get_metadata(const char* path) const {
    if (metadatas.contains(path)) {
        return metadatas.at(path);
    }
    return std::unexpected(nautix_error::directory_not_found);
}
