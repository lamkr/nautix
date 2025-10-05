#include "infra/DirectoriesLister.h"
#include "infra/fs.h"
#include <sys/stat.h>
#include <algorithm>

#include "infra/time.h"

namespace nautix::infra
{
    [[nodiscard]] std::expected<domain::Directory, std::error_code>
    build_directory(const char* path) {
        struct stat st{};
        if (stat(path, &st)) {
            return std::unexpected(std::error_code(errno, std::generic_category()));
        }

        if (!S_ISDIR(st.st_mode)) {
            return std::unexpected(std::make_error_code(std::errc::not_a_directory));
        }

        return domain::Directory(
            path,
            std::filesystem::path(path).filename(),
            compute_directory_size(path),
            get_owner_from_id(st),
            get_creation_time(st),
            get_modification_time(st),
            get_access_time(st)
        );
    }

    void sort_directories_vector(std::vector<domain::Directory>& directories, application::SortOrder order) {
        std::ranges::sort(directories,
            [order](const domain::Directory& a, const domain::Directory& b) {
                using enum application::SortOrder;
                switch (order) {
                    case BySize:
                        return a.size() < b.size();
                    case ByOwner:
                        return a.owner().name() < b.owner().name();
                    case ByCreationDate:
                        return a.creation_time() < b.creation_time();
                    case ByModificationDate:
                        return a.modification_time() < b.modification_time();
                    case ByAccessDate:
                        return a.access_time() < b.access_time();
                    case ByName:
                    case None:
                    default:
                        return a.name() < b.name();
                }
            });
    }

    std::expected<std::vector<domain::Directory>, std::error_code>
    DirectoriesLister::list_directories(const std::filesystem::path& path, application::SortOrder order)
    const {
        std::vector<domain::Directory> directories;

        try {
            std::error_code error_code;
            if (!std::filesystem::exists(path, error_code)) {
                return std::unexpected(std::make_error_code(std::errc::no_such_file_or_directory));
            }
            if (!std::filesystem::is_directory(path, error_code)) {
                return std::unexpected(std::make_error_code(std::errc::not_a_directory));
            }

            for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path, error_code)) {
                if (error_code) {
                    // Erros podem ocorrer durante a iteração, por exemplo, por falta de permissão
                    return std::unexpected(error_code);
                }

                if (entry.is_directory()) {
                    std::expected<domain::Directory, std::error_code> directory = build_directory(entry.path().c_str());
                    if (error_code) {
                        // Erros podem ocorrer durante a iteração, por exemplo, por falta de permissão
                        return std::unexpected(error_code);
                    }

                    //directories.emplace_back(std::move(directory).value());
                    directories.push_back(std::move(directory.value()));
                }
            }

        } catch (const std::filesystem::filesystem_error& e) {
            return std::unexpected(e.code());
        }

        sort_directories_vector(directories, order);

        return directories;
    }
/*

    [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code>
    //DirectoriesLister::
    list_directories_old(const std::filesystem::path& path, const application::SortOrder order)
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
                        return a.owner.name() < b.owner.name();
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
                    dynamic_cast<const domain::LocalTime&&>(metadata.creation_time),
                    dynamic_cast<const domain::LocalTime&&>(metadata.modification_time),
                    dynamic_cast<const domain::LocalTime&&>(metadata.access_time)
                };
            }
        );

        return directories;
    }*/

}
