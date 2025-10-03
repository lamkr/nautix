#include "list_directories.h"

namespace nautix::application {

    [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code>
    ListDirectories::execute(const std::filesystem::path& path, SortOrder order) const {
        return lister_->list_directories(path, order);
    }
}