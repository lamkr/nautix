#pragma once

#include <expected>
#include <filesystem>

namespace nautix::application::ports {

    class IItemRenamer {
    public:
        virtual ~IItemRenamer() = default;

        /**
         * @brief Renames a item from the file system.
         * @param oldPath Item's path.
         * @param newPath
         * @returns Error or success.
         */
        virtual std::expected<bool, std::error_code> renameItem(
            const std::filesystem::path& oldPath, const std::filesystem::path& newPath) = 0;
    };

} // namespace nautix::application