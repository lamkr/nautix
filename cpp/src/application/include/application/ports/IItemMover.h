#pragma once

#include <expected>
#include <filesystem>

namespace nautix::application::ports {

    class IItemMover {
    public:
        virtual ~IItemMover() = default;

        /**
         * @brief Moves a item from the file system.
         * @param sourcePath Item's source path.
         * @param targetPath Target path to move item.
         * @returns Error or success.
         */
        virtual std::expected<bool, std::error_code>
            moveItem(const std::filesystem::path& sourcePath, const std::filesystem::path& targetPath) = 0;
    };

} // namespace nautix::application