#pragma once
#include <expected>
#include <filesystem>

#include "../SortOrder.h"
#include "domain/directory.h"

namespace nautix::application {

    class IDirectoriesLister {
    public:
        virtual ~IDirectoriesLister() = default;

        [[nodiscard]] virtual std::expected<std::vector<domain::Directory>, std::error_code> list_directories(
            const std::filesystem::path& path,
            SortOrder order) const = 0;
    };
}
