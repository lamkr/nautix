#include "application/CopyItem.h"

namespace nautix::application {
    [[nodiscard]] std::expected<bool, std::error_code>
        CopyItem::execute(
            const std::filesystem::path& sourcePath,
            const std::filesystem::path& targetPath,
            const std::filesystem::copy_options& options ) const {
        return copier_->copyItem(sourcePath, targetPath, options);
    }
}
