#pragma once
#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>

#include "IDirectoriesLister.h"
#include "domain/directory.h"

namespace nautix::application {

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
