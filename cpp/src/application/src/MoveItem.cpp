#include "application/MoveItem.h"

namespace nautix::application {

    [[nodiscard]] std::expected<int, std::error_code>
        MoveItem::execute(
            const std::vector<std::filesystem::path>& sourcePaths,
            const std::filesystem::path& targetPath ) const {
        return mover_->moveItems(sourcePaths, targetPath);
    }
}
