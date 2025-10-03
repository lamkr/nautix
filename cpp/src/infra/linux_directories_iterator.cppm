module;

#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>

export module nautix.infra:LinuxDirectoriesIterator;

import nautix.domain;
import nautix.application;

export class LinuxDirectoriesIterator : public IDirectoriesIterator {
public:
    ~LinuxDirectoriesIterator() override = default;

    std::expected<std::vector<Directory>, std::error_code> list_directories(
        const std::filesystem::path& path,
        SortOrder order) override;
};

