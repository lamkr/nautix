#pragma once
#include <expected>

#include "application/ports/IItemDeleter.h"

namespace nautix::infra {

    class ItemDeleter final : public application::ports::IItemDeleter {
    public:
        std::expected<bool, std::error_code> deleteItem(const std::filesystem::path& path) override;
    };

}
