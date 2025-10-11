#include "infra/ItemMover.h"
#include "common/Logger.h"
#include "infra/errors.h"
#include <fmt/std.h>
#include <system_error>

namespace nautix::infra {
    std::expected<void, std::error_code> ItemMover::moveItem(
        const std::filesystem::path& sourcePath,
        const std::filesystem::path& targetPath) {
        std::error_code error_code;

        // Verificação fundamental: a origem existe?
        if (!std::filesystem::exists(sourcePath, error_code)) {
            Logger::get()->error("Move failed: Source path '{}' does not exist.", sourcePath);
            return std::unexpected(std::make_error_code(std::errc::no_such_file_or_directory));
        }

        // Check move of item to itself.
        if (std::filesystem::exists(targetPath) && std::filesystem::equivalent(sourcePath, targetPath, error_code)) {
            Logger::get()->error("Move failed: Source and target path '{}' are the same.", sourcePath);
            if (std::filesystem::is_directory(sourcePath)) {
                return std::unexpected(make_error_code(nautix_error::move_directory_to_itself));
            }
            return std::unexpected(make_error_code(nautix_error::move_file_to_itself, error_code));
        }

        std::filesystem::path finalTargetPath;
        if (std::filesystem::is_directory(targetPath, error_code)) {
            // If target is a directory, the target is inside it.
            finalTargetPath = targetPath / sourcePath.filename();
        }
        else {
            // If target does not exist or is a file, the target is itself (rename).
            finalTargetPath = targetPath;
        }

        // Check move a directory to an existing file.
        if (std::filesystem::is_directory(sourcePath) && std::filesystem::is_regular_file(finalTargetPath, error_code)) {
            Logger::get()->error("Move failed: Cannot move a directory '{}' onto file '{}'.", sourcePath,
                finalTargetPath);
            return std::unexpected(make_error_code(nautix_error::move_directory_to_file, error_code));
        }

        // Check if final target already exists.
        if (std::filesystem::exists(finalTargetPath, error_code)) {
            // The rule is not overwrite it.
            Logger::get()->error("Move failed: Target path '{}' already exists.", finalTargetPath);
            return std::unexpected(std::make_error_code(std::errc::file_exists));
        }

        // Check move a directory to itself.
        if (std::filesystem::is_directory(sourcePath)) {
            auto parent = finalTargetPath.parent_path();
            while (!parent.empty() && parent != parent.root_path()) {
                if (std::filesystem::equivalent(parent, sourcePath, error_code)) {
                    Logger::get()->error(
                        "Move failed: Cannot move directory '{}' into a subdirectory of itself '{}'.",
                        sourcePath, targetPath);
                    return std::unexpected(make_error_code(nautix_error::move_directory_to_itself));
                }
                parent = parent.parent_path();
            }
        }

        error_code.clear();

        std::filesystem::rename(sourcePath, finalTargetPath, error_code);
        if (!error_code) {
            return {}; // Sucesso!
        }

        if (error_code == std::errc::cross_device_link) {
            Logger::get()->info("Move information: Cross-device move for '{}' to '{}'. Falling back to copy and delete.",
                sourcePath, finalTargetPath);

            constexpr auto copy_options = std::filesystem::copy_options::recursive;
            std::filesystem::copy(sourcePath, finalTargetPath, copy_options, error_code);

            if (error_code) {
                Logger::get()->error("Move failed: Fallback copy failed for '{}' to '{}': {}-{}",
                    sourcePath, finalTargetPath, error_code.value(), error_code.message());
                return std::unexpected(error_code);
            }

            std::filesystem::remove_all(sourcePath, error_code);
            if (error_code) {
                Logger::get()->critical("Move failed: Source cleanup failed for {}: {}-{}",
                    sourcePath, error_code.value(), error_code.message());
                return std::unexpected(make_error_code(nautix_error::move_cleanup_failed, error_code));
            }

            return {};
        }

        Logger::get()->error("Move failed: '{}' to '{}': {}-{}", sourcePath, finalTargetPath,
            error_code.value(), error_code.message());
        return std::unexpected(error_code);
    }
}
