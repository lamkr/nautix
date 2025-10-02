
import nautix.domain;
import nautix.application;

#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>

[[nodiscard]] std::expected<std::vector<Directory>, std::error_code>
ListDirectories::execute(
    const std::filesystem::path& path,
    SortOrder order) const
{
    return {};//ListDirectories
}

