#include <filesystem>
#include <fstream>
#include <chrono>
#include <memory>
#include <catch2/catch_all.hpp>

#include "TempDir.h"
#include "application/MoveItem.h"
#include "common/Logger.h"
#include "infra/errors.h"
#include "infra/ItemMover.h"

using namespace std::chrono_literals;
using namespace nautix::application;

TEST_CASE("MoveItem use case [integration test]", "[application][use_case]") {

    SECTION("Move an existing file to a non-existent file in the same directory") {
        // Arrange
        const TempDir tmp;

        const auto sourcePath = tmp.path / "subdir1";
        std::filesystem::create_directory(sourcePath);

        const auto filename1 = "file1.txt";
        const auto filename2 = "file2.txt";
        std::ofstream(sourcePath / filename1).put('a');

        const auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        // Act
        const auto result = useCase.execute(sourcePath / filename1, sourcePath / filename2 );

        // Assert
        REQUIRE(result.has_value());
        REQUIRE_FALSE(std::filesystem::exists(sourcePath / filename1));
        REQUIRE(std::filesystem::exists(sourcePath / filename2));
    }

    SECTION("Move an existing file to itself causes error") {
        // Arrange
        const TempDir tmp;
        const auto expected_error = make_error_code(nautix_error::move_file_to_itself);

        const auto sourcePath = tmp.path / "subdir1";
        std::filesystem::create_directory(sourcePath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        const auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        // Act
        const auto result = useCase.execute(sourcePath / filename, sourcePath / filename);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("Move a file to another directory") {
        // Arrange
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(targetPath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        const auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        // Act
        const auto result = useCase.execute(sourcePath / filename, targetPath );

        // Assert
        REQUIRE(result.has_value());
        REQUIRE_FALSE(std::filesystem::exists(sourcePath / filename));
        REQUIRE(std::filesystem::exists(targetPath / filename));
    }

    SECTION("Move a directory to another directory") {
        // Arrange
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(targetPath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        const auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        // Act
        const auto result = useCase.execute(sourcePath, targetPath);

        // Assert
        REQUIRE(result.has_value());
        REQUIRE_FALSE(std::filesystem::exists(sourcePath));
        // New subdiretory exists: /tmp/x/subdir2/subdir1
        std::filesystem::path targetPathComplete = targetPath / sourcePath.filename();
        REQUIRE(std::filesystem::exists(targetPathComplete));
        // File in new subdiretory exists: /tmp/x/subdir2/subdir1/file.txt
        targetPathComplete /= filename;
        REQUIRE(std::filesystem::exists(targetPathComplete));
    }

    SECTION("Move a directory to its subdirectory causes error") {
        // Arrange
        const TempDir tmp;
        const auto expected_error = make_error_code(nautix_error::move_directory_to_itself);

        const auto sourcePath = tmp.path / "dir";
        const auto targetPath = sourcePath / "subdir";
        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(targetPath);

        const auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        // Act
        const auto result = useCase.execute(sourcePath, targetPath);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }
}

