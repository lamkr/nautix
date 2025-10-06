#pragma once
#include <expected>

#include "application/ports/IItemMover.h"

namespace nautix::infra {

    class ItemMover final : public application::ports::IItemMover {
    public:
        std::expected<bool, std::error_code>
            moveItem(
                const std::filesystem::path& sourcePath,
                const std::filesystem::path& targetPath) override;
    };

}
