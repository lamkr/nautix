#pragma once

#include "ports/IItemCopier.h"

#include <memory>
#include <utility>
#include <filesystem>

namespace nautix::application {

    class CopyItem final {
        std::shared_ptr<ports::IItemCopier> copier_;
    public:
        explicit CopyItem(std::shared_ptr<ports::IItemCopier> copier)
            : copier_(std::move(copier)) {}

        [[nodiscard]] std::expected<bool, std::error_code>
            execute(
                const std::filesystem::path& sourcePath,
                const std::filesystem::path& targetPath,
                const std::filesystem::copy_options& options) const;
    };

}