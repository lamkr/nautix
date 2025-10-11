#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>
#include <catch2/catch_all.hpp>

#include "TempDir.h"
#include "application/RenameItem.h"
#include "infra/errors.h"
#include "infra/ItemRenamer.h"

using namespace std::chrono_literals;
using namespace nautix::application;

TEST_CASE("RenameItem use case [integration test]", "[application][use_case]") {

    SECTION("should rename an existing file") {
        // Arrange
        const TempDir tmp;

        const auto oldPath = tmp.path / "old_file.txt";
        const auto newPath = tmp.path / "new_file.txt";

        // Create a file (should not appear in the directory listing)
        std::ofstream(oldPath).put('a');

        auto renamer = std::make_shared<nautix::infra::ItemRenamer>();
        RenameItem useCase(renamer);

        // Act
        const auto result = useCase.execute(oldPath, newPath);

        // Assert
        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE_FALSE(std::filesystem::exists(oldPath));
        REQUIRE(std::filesystem::exists(newPath));
    }

    SECTION("should fail renaming a file with same name") {
        // Arrange
        const auto expected_error = std::make_error_code(std::errc::file_exists);
        const TempDir tmp;

        const auto filename = "old_file.txt";
        const auto oldPath = tmp.path / filename;

        std::ofstream(oldPath).put('a');

        auto renamer = std::make_shared<nautix::infra::ItemRenamer>();
        RenameItem useCase(renamer);

        // Act
        const auto result = useCase.execute(oldPath, oldPath);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should fail renaming an existing file to an existing directory") {
        const auto expected_error = std::make_error_code(std::errc::is_a_directory);
        // Arrange
        const TempDir tmp;

        const auto oldPath = tmp.path / "old_file";
        const auto newPath = tmp.path / "new_file";

        std::ofstream(oldPath).put('a');

        std::filesystem::create_directory(newPath);

        auto renamer = std::make_shared<nautix::infra::ItemRenamer>();
        RenameItem useCase(renamer);

        // Act
        const auto result = useCase.execute(oldPath, newPath);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should rename an existing directory") {
        // Arrange
        const TempDir tmp;

        const auto oldPath = tmp.path / "subdir1";
        const auto newPath = tmp.path / "subdir2";
        std::filesystem::create_directory(oldPath);

        auto renamer = std::make_shared<nautix::infra::ItemRenamer>();
        RenameItem useCase(renamer);

        // Act
        const auto result = useCase.execute(oldPath, newPath);

        // Assert
        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE_FALSE(std::filesystem::exists(oldPath));
        REQUIRE(std::filesystem::exists(newPath));
    }

    SECTION("should fail renaming a directory with same name") {
        // Arrange
        const auto expected_error = make_error_code(nautix_error::directory_exists);
        const TempDir tmp;

        const auto oldPath = tmp.path / "dir";
        std::filesystem::create_directory(oldPath);

        auto renamer = std::make_shared<nautix::infra::ItemRenamer>();
        RenameItem useCase(renamer);

        // Act
        const auto result = useCase.execute(oldPath, oldPath);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should fail renaming an existing directory to an existing file") {
        const auto expected_error = std::make_error_code(std::errc::not_a_directory);
        // Arrange
        const TempDir tmp;

        const auto oldPath = tmp.path / "old_file";
        const auto newPath = tmp.path / "new_file";

        std::filesystem::create_directory(oldPath);
        std::ofstream(newPath).put('a');

        auto renamer = std::make_shared<nautix::infra::ItemRenamer>();
        RenameItem useCase(renamer);

        // Act
        const auto result = useCase.execute(oldPath, newPath);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

}

