module;

#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>

module nautix.application;

import :list_directories;
import nautix.domain;

[[nodiscard]] std::expected<std::vector<Directory>, std::error_code>
ListDirectories::execute(const std::filesystem::path& path, SortOrder order) const {
    return iterator_->list_directories(path, order);
}

