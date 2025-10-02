module;

#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>

export module nautix.application:list_directories;

import nautix.domain;

export enum class SortOrder {
    None,
    ByName,
    BySize,
    ByOwner,
    ByCreationDate,
    ByModificationDate,
    ByAccessDate,
};

export class IDirectoriesIterator {
public:
    virtual ~IDirectoriesIterator() = default;

    virtual std::expected<Directory, std::error_code> create(
        const std::filesystem::path& path,
        SortOrder order) = 0;
};

export class ListDirectories {
public:
    explicit ListDirectories(std::shared_ptr<IDirectoriesIterator> iterator)
        : iterator_(std::move(iterator)) {}
    
    [[nodiscard]] std::expected<std::vector<Directory>, std::error_code> execute(
        const std::filesystem::path& path,
        SortOrder order = SortOrder::None) const;

private:
    std::shared_ptr<IDirectoriesIterator> iterator_;
};