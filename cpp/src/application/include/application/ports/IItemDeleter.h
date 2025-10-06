#pragma once

#include <filesystem>
#include <expected>

namespace nautix::application::ports {

    class IItemDeleter {
    public:
        virtual ~IItemDeleter() = default;

        /**
         * @brief Delete an item from the file system.
         * @param path The path of item to be deleted.
         * @returns Error or success.
         */
        virtual std::expected<bool, std::error_code>
            deleteItem(const std::filesystem::path& path) = 0;
    };

}