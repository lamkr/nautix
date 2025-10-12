#pragma once
#include <expected>

#include "application/ports/IItemCopier.h"

namespace nautix::infra {

    class ItemCopier final : public application::ports::IItemCopier {
    public:
        std::expected<bool, std::error_code>
            copyItem(
                const std::filesystem::path& sourcePath,
                const std::filesystem::path& targetPath,
                const std::filesystem::copy_options& options) override;
    };

}
