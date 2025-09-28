#pragma once
#include <vector>
#include "directory.h"
#include "sort_order.h"

namespace nautix::application {

    using namespace nautix::domain;

    class ListDirectories {
    public:
        // Run the use case from a source directory
        [[nodiscard]] std::vector<Directory> execute(
            const Directory& root,
            SortOrder order = SortOrder::None
        ) const;
    };
} // namespace nautix::application
