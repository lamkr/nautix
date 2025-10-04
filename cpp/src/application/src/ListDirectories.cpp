#include "ListDirectories.h"
#include <expected>
#include "domain/directory.h"

namespace nautix::application {

    [[nodiscard]] std::expected<std::vector<domain::Directory>, std::error_code>
    ListDirectories::execute(const std::filesystem::path& path, const SortOrder order) const {
        return lister_->list_directories(path, order);
    }
}
