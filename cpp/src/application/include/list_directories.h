#pragma once
#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>
#include "../../include/domain/directory.h"

namespace nautix::application {

    enum class SortOrder {
        None,
        ByName,
        BySize,
        ByOwner,
        ByCreationDate,
        ByModificationDate,
        ByAccessDate,
    };

    class IDirectoriesLister {
    public:
        virtual ~IDirectoriesLister() = default;

        [[nodiscard]] virtual std::expected<std::vector<domain::Directory>, std::error_code> list_directories(
            const std::filesystem::path& path,
            SortOrder order) const = 0;
    };

    class ListDirectories final {
        std::shared_ptr<IDirectoriesLister> lister_;
    public:
        explicit ListDirectories(std::shared_ptr<IDirectoriesLister> lister)
            : lister_(std::move(lister)) {}

        [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code> execute(
            const std::filesystem::path& path,
            SortOrder order = SortOrder::None) const;
    };
}
