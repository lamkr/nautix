#pragma once

#include <expected>
#include <filesystem>

namespace nautix::application::ports {

    class IItemCopier {
    public:
        virtual ~IItemCopier() = default;

        /**
         * @brief Copies a item from the file system.
         * @param sourcePath Item's source path.
         * @param targetPath Target path to copy item.
         * @param options
         * @returns Error or success.
         */
        virtual std::expected<bool, std::error_code>
            copyItem(const std::filesystem::path& sourcePath, const std::filesystem::path& targetPath, const std::filesystem::copy_options&
                options) = 0;
    };

} // namespace nautix::application