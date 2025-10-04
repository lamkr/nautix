#pragma once
#include "../../include/domain/directory.h"
#include "../../src/application/include/list_directories.h"
#include "../../src/infra/src/ports/directories_lister.h"

class MockDirectoriesLister final : public nautix::application::IDirectoriesLister {
    std::vector<nautix::infra::DirectoryMetadata>& metadatas;
public:
    explicit MockDirectoriesLister(std::vector<nautix::infra::DirectoryMetadata>& metadatas) : metadatas(metadatas) {}
    ~MockDirectoriesLister() override = default;

    [[nodiscard]] std::expected<std::vector<nautix::domain::Directory>, std::error_code>
        list_directories( const std::filesystem::path& path, nautix::application::SortOrder order) const override;
};
