#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>

import nautix.domain;
import nautix.application:list_directories;

[[nodiscard]] std::expected<std::vector<Directory>, std::error_code>
ListDirectories::execute(
    const std::filesystem::path& path,
    SortOrder order) const
{
    return {};//ListDirectories
}

