#include "infra/ItemRenamer.h"
#include "common/Logger.h"
#include "infra/errors.h"
#include <fmt/std.h>
#include <system_error>

namespace nautix::infra {
    std::expected<bool, std::error_code> ItemRenamer::renameItem(
        const std::filesystem::path& oldPath,
        const std::filesystem::path& newPath) {
        std::error_code error_code;

        // Check rename of item to itself.
        if (std::filesystem::exists(newPath) && std::filesystem::equivalent(oldPath, newPath, error_code)) {
            Logger::get()->error("Rename failed: Source and target path '{}' are the same.", oldPath);
            if (std::filesystem::is_directory(oldPath)) {
                return std::unexpected(make_error_code(nautix_error::directory_exists, error_code));
            }
            return std::unexpected(std::make_error_code(std::errc::file_exists));
        }

        std::filesystem::rename(oldPath, newPath, error_code);
        if (error_code) {
            return std::unexpected(error_code);
        }
        return true;
    }
}
