#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>
#include <catch2/catch_all.hpp>

#include "TempDir.h"
#include "application/CopyItem.h"
#include "infra/errors.h"
#include "infra/ItemCopier.h"

using namespace std::chrono_literals;
using namespace nautix::application;

TEST_CASE("CopyItem use case [integration test]", "[application][use_case]") {

    SECTION("should copy a file to another directory") {
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(targetPath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        std::filesystem::copy_options options{};
        const auto result = useCase.execute(sourcePath / filename, targetPath / filename, options);

        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE(std::filesystem::exists(targetPath / filename));
    }

    SECTION("should copy a file with new name to same directory") {
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        std::filesystem::create_directory(sourcePath);

        const auto filename1 = "file.txt";
        const auto filename2 = "copy of file.txt";
        std::ofstream(sourcePath / filename1).put('a');

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        std::filesystem::copy_options options{};
        const auto result = useCase.execute(sourcePath / filename1, sourcePath / filename2, options);

        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE(std::filesystem::exists(sourcePath / filename2));
    }

    SECTION("should fail when copy a file to non-existent directory") {
        // Arrange
        const auto expected_error = std::make_error_code(std::errc::no_such_file_or_directory);
        const TempDir tmp;

        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(targetPath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        std::filesystem::copy_options options{};

        // Act
        const auto result = useCase.execute(
            sourcePath / filename,
            targetPath / "non-existent" / filename,
            options);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should fail when copy a file to itself") {
        // Arrange
        const auto expected_error = std::make_error_code(std::errc::file_exists);
        const TempDir tmp;

        const auto sourcePath = tmp.path / "subdir1";
        std::filesystem::create_directory(sourcePath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        std::filesystem::copy_options options{};

        // Act
        const auto result = useCase.execute(
            sourcePath / filename,
            sourcePath / filename,
            options);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should fail when copy a file to unaccessible directory") {
        if ( geteuid() == 0 ) FAIL("Current user cannot be root");

        // Arrange
        const auto expected_error = std::make_error_code(std::errc::permission_denied);
        const TempDir tmp;

        const auto sourcePath = tmp.path / "subdir1";
        const std::filesystem::path targetPath = "/usr/bin";
        std::filesystem::create_directory(sourcePath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        std::filesystem::copy_options options{};

        // Act
        const auto result = useCase.execute(
            sourcePath / filename,
            targetPath / filename,
            options);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should copy a directory to another directory") {
        // Arrange
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        constexpr std::filesystem::copy_options options{};
        // Act
        const auto result = useCase.execute(sourcePath, targetPath, options);

        // Assert
        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE(std::filesystem::exists(targetPath));
        REQUIRE(std::filesystem::exists(targetPath / filename));
    }

    SECTION("should fail when recursively copying a directory to a subdirectory of it") {
        // Arrange
        const auto expected_error = std::make_error_code(std::errc::filename_too_long);
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "dir";
        const auto targetPath = sourcePath / "subdir";
        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(targetPath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        constexpr auto options{std::filesystem::copy_options::recursive};
        // Act
        const auto result = useCase.execute(sourcePath, targetPath, options);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should fail when copy a directory to itself") {
        // Arrange
        const auto expected_error = std::make_error_code(std::errc::file_exists);
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "dir";
        std::filesystem::create_directory(sourcePath);

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        // Act
        const auto result = useCase.execute(sourcePath, sourcePath, {});

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

}

