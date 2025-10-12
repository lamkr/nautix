#include "application/RenameItem.h"

namespace nautix::application {
    [[nodiscard]] std::expected<bool, std::error_code> RenameItem::execute(const std::filesystem::path& oldPath, const std::filesystem::path& newPath) const {
        return renamer_->renameItem(oldPath, newPath);
    }
}
