#include "fs.h"
#include "../domain/directory.h"
#include "directories_lister.h"

#include <algorithm>

namespace nautix::infra {

    [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code>
    DirectoriesLister::list_directories(const std::filesystem::path& path, application::SortOrder order)
    const
    {
        std::vector<DirectoryMetadata> metadatas;

        for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_directory()) {
                std::expected<DirectoryMetadata, std::error_code> metadata =
                    get_metadata(entry.path().c_str());
                if (!metadata.has_value()) {
                    return std::unexpected(metadata.error());
                }
                metadatas.push_back(metadata.value());
            }
        }

        sort_metadata_vector(metadatas, order);

        return to_directories(metadatas);
        /*std::vector<domain::Directory> directories;

        std::ranges::transform(metadatas,
            std::back_inserter(directories),
            [](DirectoryMetadata& metadata) {
                return domain::Directory {
                    metadata.path,
                    std::move(metadata.name),
                    metadata.size,
                    std::move(metadata.owner),
                    std::move(metadata.creation_time),
                    std::move(metadata.modification_time),
                    std::move(metadata.access_time)
                };
            });

        return directories;*/
    }

    [[nodiscard]] std::expected<DirectoryMetadata, std::error_code> get_metadata(const char* path) {
        struct stat st{};
        if (stat(path, &st)) {
            return std::unexpected(std::error_code(errno, std::generic_category()));
        }

        if (!S_ISDIR(st.st_mode)) {
            return std::unexpected(std::make_error_code(std::errc::not_a_directory));
        }

        return DirectoryMetadata(
            path,
            std::filesystem::path(path).filename(),
            compute_directory_size(path),
            get_owner_from_id(st),
            get_creation_time(st),
            get_modification_time(st),
            get_access_time(st)
        );
    }

    void sort_metadata_vector(std::vector<DirectoryMetadata>& metadatas, application::SortOrder order) {
        std::ranges::sort(metadatas,
            [order](const DirectoryMetadata& a, const DirectoryMetadata& b) {
                using enum application::SortOrder;
                switch (order) {
                    case BySize:
                        return a.size < b.size;
                    case ByOwner:
                        return a.owner.id() < b.owner.id();
                    case ByCreationDate:
                        return a.creation_time < b.creation_time;
                    case ByModificationDate:
                        return a.modification_time < b.modification_time;
                    case ByAccessDate:
                        return a.access_time < b.access_time;
                    case ByName:
                    case None:
                    default:
                        return a.owner.name() < b.owner.name();
                }
            });
    }

    std::vector<domain::Directory> to_directories(std::vector<DirectoryMetadata>& metadatas) {
        std::vector<domain::Directory> directories;
        directories.reserve(metadatas.size());

        std::ranges::transform(
            metadatas,
            std::back_inserter(directories),
            [](DirectoryMetadata& metadata) {
                return domain::Directory{
                    metadata.path,
                    std::move(metadata.name),
                    metadata.size,
                    std::move(metadata.owner),
                    std::move(metadata.creation_time),
                    std::move(metadata.modification_time),
                    std::move(metadata.access_time)
                };
            }
        );

        return directories;
    }

}
