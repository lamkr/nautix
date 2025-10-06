#include "infra/ItemRenamer.h"

#include <system_error>

namespace nautix::infra {
    std::expected<bool, std::error_code> ItemRenamer::renameItem(const std::filesystem::path& oldPath, const std::filesystem::path& newPath) {
        std::error_code error_code;
        std::filesystem::rename(oldPath, newPath, error_code);
        if (error_code) {
            return std::unexpected(error_code);
        }
        return true;
    }
}
