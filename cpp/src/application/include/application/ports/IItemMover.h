#pragma once

#include <expected>
#include <filesystem>
#include <vector>

namespace nautix::application::ports {

    class IItemMover {
    public:
        virtual ~IItemMover() = default;

        /**
         * @brief Moves one or more items from the file system.
         * @param sourcePaths Source path of items.
         * @param targetPath Target path to move items.
         * @returns Error or total moved items.
         */
        virtual std::expected<int, std::error_code>
            moveItems(
                const std::vector<std::filesystem::path>& sourcePaths,
                const std::filesystem::path& targetPath) = 0;
    };

} // namespace nautix::application