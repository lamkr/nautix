#pragma once

#include "ports/IItemRenamer.h"

#include <memory>
#include <utility>
#include <filesystem>

namespace nautix::application {

    class RenameItem final {
        std::shared_ptr<ports::IItemRenamer> renamer_;
    public:
        explicit RenameItem(std::shared_ptr<ports::IItemRenamer> renamer)
            : renamer_(std::move(renamer)) {}

        [[nodiscard]] std::expected<bool, std::error_code> execute(const std::filesystem::path& oldPath, const std::filesystem::path& newPath) const;
    };

}