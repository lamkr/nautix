#pragma once

#include <vector>
#include <filesystem>
#include <stdexcept>

namespace nautix::application::ports {

    /**
     * @brief Exception thrown when an item deletion fails.
     */
    class DeletionFailure final : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * @brief Interface for deleting file system items.
     * This is a port in the application layer, to be implemented by the
     * infrastructure layer.
     */
    class IItemsDeleter {
    public:
        virtual ~IItemsDeleter() = default;

        /**
         * @brief Deletes a list of items from the file system.
         * @param paths A vector of paths to the items to be deleted.
         * @throws DeletionFailure if one or more items could not be deleted.
         */
        virtual void deleteItems(const std::vector<std::filesystem::path>& paths) = 0;
    };

} // namespace nautix::application