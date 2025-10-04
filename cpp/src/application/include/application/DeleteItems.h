#pragma once

#include "ports/IItemsDeleter.h"

#include <memory>
#include <utility>
#include <vector>
#include <filesystem>

namespace nautix::application {

    class DeleteItems final {
        std::shared_ptr<ports::IItemsDeleter> deleter_;
    public:
        explicit DeleteItems(std::shared_ptr<ports::IItemsDeleter> deleter)
            : deleter_(std::move(deleter)) {}

        void execute(const std::vector<std::filesystem::path>& paths) const;
    };

}