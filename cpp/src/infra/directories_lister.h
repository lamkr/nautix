#pragma once
#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>
#include "../application/list_directories.h"

namespace nautix::infra {
    struct DirectoryMetadata;

    class DirectoriesLister final : public application::IDirectoriesLister {
    public:
        ~DirectoriesLister() override = default;

        [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code>
            list_directories( const std::filesystem::path& path, application::SortOrder order) const override;

    };

    [[nodiscard]] std::expected<DirectoryMetadata, std::error_code> get_metadata(const char* path);

    void sort_metadata_vector(std::vector<DirectoryMetadata>& metadatas, application::SortOrder order);

    std::vector<domain::Directory> to_directories(std::vector<DirectoryMetadata>& metadatas);

    struct DirectoryMetadata {
        std::filesystem::path path;
        std::string name;
        std::uintmax_t size{0};
        domain::Owner owner{};
        domain::LocalTime creation_time;
        domain::LocalTime modification_time;
        domain::LocalTime access_time;
    };
}
