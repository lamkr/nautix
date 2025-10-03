#pragma once
#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>
#include "../application/list_directories.h"

namespace nautix::infra {
    class DirectoriesLister : public application::IDirectoriesLister {
    public:
        ~DirectoriesLister() override = default;

        [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code>
            list_directories( const std::filesystem::path& path, application::SortOrder order) const override;
    };
}