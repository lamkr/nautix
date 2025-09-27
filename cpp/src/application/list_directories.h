#pragma once
#include <vector>
#include "../domain/directory.h"

namespace nautix::application {

    class ListDirectories {
    public:
        // Run the use case from a source directory
        [[nodiscard]] std::vector<domain::Directory> execute(
            const domain::Directory& root) const;
    };

} // namespace nautix::application
