#pragma once
#include <expected>

#include "application/ports/IItemMover.h"

namespace nautix::infra {

    class ItemMover final : public application::ports::IItemMover {
    public:
        std::expected<int, std::error_code>
            moveItems(
                const std::vector<std::filesystem::path>& sourcePaths,
                const std::filesystem::path& targetPath) override;
    };

}
