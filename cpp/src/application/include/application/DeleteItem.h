#pragma once

#include <expected>

#include "ports/IItemDeleter.h"

#include <memory>
#include <utility>
#include <filesystem>

namespace nautix::application {

    class DeleteItem final {
        std::shared_ptr<ports::IItemDeleter> deleter_;
    public:
        explicit DeleteItem(std::shared_ptr<ports::IItemDeleter> deleter)
            : deleter_(std::move(deleter)) {}

        std::expected<void, std::error_code> execute(const std::filesystem::path& path) const;
    };

}