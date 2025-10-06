#pragma once
#include <expected>

#include "application/ports/IItemRenamer.h"

namespace nautix::infra {

    class ItemRenamer final : public application::ports::IItemRenamer {
    public:
        std::expected<bool, std::error_code> renameItem(const std::filesystem::path& oldPath, const std::filesystem::path& newPath) override;
    };

}
