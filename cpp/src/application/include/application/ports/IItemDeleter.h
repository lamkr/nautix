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
         * @return An empty expected on success, or an error_code on failure.
         */
        virtual std::expected<void, std::error_code>
            deleteItem(const std::filesystem::path& path) = 0;
    };

}