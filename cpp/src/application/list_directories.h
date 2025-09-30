#pragma once
#include <vector>
#include "directory.h"
#include "sort_order.h"

namespace nautix::application {

    class ListDirectories {
    public:
        [[nodiscard]] std::vector<domain::Directory> execute(
            const std::string&& existing_path,
            SortOrder order = SortOrder::None
        ) const;

        [[nodiscard]] std::vector<domain::Directory> execute(
            const domain::Directory& directory,
            SortOrder order = SortOrder::None
        ) const;
    };
}
