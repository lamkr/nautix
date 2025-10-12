#include "application/MoveItem.h"

namespace nautix::application {

    [[nodiscard]] std::expected<void, std::error_code>
        MoveItem::execute(
            const std::filesystem::path& sourcePath,
            const std::filesystem::path& targetPath ) const {
        return mover_->moveItem(sourcePath, targetPath);
    }
}
