#pragma once
#include "application/ports/IItemsDeleter.h"

namespace nautix::infra {

    class ItemsDeleter final : public application::ports::IItemsDeleter {
    public:
        void deleteItems(const std::vector<std::filesystem::path>& paths) override;
    };

}
