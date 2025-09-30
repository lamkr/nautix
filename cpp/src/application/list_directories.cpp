#include <vector>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <format>
#include "list_directories.h"
#include "../core/fs.h"

namespace nautix::application {
    namespace fs = std::filesystem;
    namespace chrono = std::chrono;

    ListDirectories::ListDirectories(IDirectoryMetadataProvider& provider)
        : provider_(provider) {}

    std::expected<std::vector<domain::Directory>, std::error_code> ListDirectories::execute(
        const std::string& existing_path, const SortOrder order ) const
    {
        return execute(existing_path.c_str(), order);
    }

    std::expected<std::vector<domain::Directory>, std::error_code> ListDirectories::execute(const char* existing_path,
        const SortOrder order) const
    {
        std::vector<DirectoryMetadata> metadatas;

        for (const fs::directory_entry& entry : fs::directory_iterator(existing_path)) {
            std::expected<DirectoryMetadata, std::error_code> metadata =
                provider_.get_metadata(entry.path().c_str());
            if (!metadata.has_value()) {
                return std::unexpected(metadata.error());
            }
            metadatas.push_back(*metadata);
        }

        std::ranges::sort(metadatas,
            [order](const DirectoryMetadata& a, const DirectoryMetadata& b) {
                using enum SortOrder;
                switch (order) {
                    case BySize:
                        return a.size < b.size;
                    case ByOwner:
                        return a.owner_id < b.owner_id;
                    case ByCreationDate:
                        return a.creation_time < b.creation_time;
                    case ByModificationDate:
                        return a.modification_time < b.modification_time;
                    case ByAccessDate:
                        return a.access_time < b.access_time;
                    case ByName:
                    case None:
                    default:
                        return a.name < b.name;
                }
            });

        std::vector<domain::Directory> directories;

        std::ranges::transform(metadatas,
            std::back_inserter(directories),
            [](DirectoryMetadata& metadata) {
                return domain::Directory::from_metadata(std::move(metadata));
            });

        return directories;
    }
}
