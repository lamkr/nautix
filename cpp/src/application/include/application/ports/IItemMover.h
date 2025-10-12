#pragma once

#include <expected>
#include <filesystem>

namespace nautix::application::ports {

    class IItemMover {
    public:
        virtual ~IItemMover() = default;

        /**
         * @brief Moves an item from the file system.
         * @param sourcePath Source path of item.
         * @param targetPath Target path to move item.
         * @return An empty expected on success, or an error_code on failure.
         */
        virtual std::expected<void, std::error_code>
            moveItem(
                const std::filesystem::path& sourcePath,
                const std::filesystem::path& targetPath) = 0;
    };

} // namespace nautix::application