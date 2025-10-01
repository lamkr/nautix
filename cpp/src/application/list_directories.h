#pragma once
#include <vector>
#include "../domain/directory.h"
#include "sort_order.h"

namespace nautix::application {

    class ListDirectories {
        const IFileSystemProvider& provider_;
    public:
        explicit ListDirectories(IFileSystemProvider& provider);

        [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code> execute(
            const std::string& existing_path,
            SortOrder order = SortOrder::None
        ) const;

        [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code> execute(
            const char* existing_path, SortOrder order = SortOrder::None
        ) const;
    };
}
