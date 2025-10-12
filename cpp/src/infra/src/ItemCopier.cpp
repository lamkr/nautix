#include "infra/ItemCopier.h"

#include <system_error>

namespace nautix::infra {
    std::expected<bool, std::error_code>
        ItemCopier::copyItem(
            const std::filesystem::path& sourcePath,
            const std::filesystem::path& targetPath,
            const std::filesystem::copy_options& options
        )
    {
        std::error_code error_code;
        std::filesystem::copy(sourcePath, targetPath, options, error_code);
        if (error_code) {
            return std::unexpected(error_code);
        }
        return true;
    }
}
