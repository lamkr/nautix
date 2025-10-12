#pragma once

#include "ports/IItemMover.h"

#include <memory>
#include <utility>
#include <filesystem>

namespace nautix::application {

    class MoveItem final {
        std::shared_ptr<ports::IItemMover> mover_;
    public:
        explicit MoveItem(std::shared_ptr<ports::IItemMover> mover)
            : mover_(std::move(mover)) {}

        [[nodiscard]] std::expected<void, std::error_code>
            execute(
                const std::filesystem::path& sourcePath,
                const std::filesystem::path& targetPath) const;
    };

}