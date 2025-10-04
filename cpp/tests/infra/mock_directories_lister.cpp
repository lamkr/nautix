#include "mock_directories_lister.h"
#include <filesystem>
#include <system_error>
#include <expected>
#include <vector>
#include "../../src/infra/src/ports/directories_lister.h"

[[nodiscard]] std::expected<std::vector<nautix::domain::Directory>, std::error_code>
MockDirectoriesLister::list_directories(
    [[maybe_unused]] const std::filesystem::path& path,
    const nautix::application::SortOrder order)
const
{
    nautix::infra::sort_metadata_vector(metadatas, order);

    return nautix::infra::to_directories(metadatas);
}
