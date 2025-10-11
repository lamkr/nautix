#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <catch2/catch_all.hpp>

#include "TempDir.h"
#include "common/Logger.h"
#include "infra/errors.h"
#include "application/DeleteItem.h"
#include "infra/ItemDeleter.h"

using namespace std::chrono_literals;
using namespace nautix::application;

TEST_CASE("DeleteItem use case [integration test]", "[application][use_case]") {

    SECTION("should delete a file") {
        // Arrange
        const TempDir tmp;

        const auto sourcePath = tmp.path / "dir";
        std::filesystem::create_directory(sourcePath);

        const auto filepath = sourcePath / "file1.txt";
        std::ofstream(filepath).put('a');

        const auto deleter = std::make_shared<nautix::infra::ItemDeleter>();
        DeleteItem useCase(deleter);

        // Act
        const auto result = useCase.execute(filepath);

        // Assert
        REQUIRE(result.has_value());
        REQUIRE_FALSE(std::filesystem::exists(filepath));
    }

    SECTION("should return an error when file deletion fails") {
        if ( geteuid() == 0 ) FAIL("Current user cannot be root");

        // Arrange
        const auto expected_error = std::make_error_code(std::errc::permission_denied);
        const TempDir tmp;

        const auto filepath = "/usr/bin/sudo";

        const auto deleter = std::make_shared<nautix::infra::ItemDeleter>();
        DeleteItem useCase(deleter);

        // Act
        auto result = useCase.execute(filepath);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should delete a directory") {
        // Arrange
        const TempDir tmp;

        const auto sourcePath = tmp.path / "dir";
        std::filesystem::create_directory(sourcePath);

        const auto deleter = std::make_shared<nautix::infra::ItemDeleter>();
        DeleteItem useCase(deleter);

        // Act
        const auto result = useCase.execute(sourcePath);

        // Assert
        REQUIRE(result.has_value());
        REQUIRE_FALSE(std::filesystem::exists(sourcePath));
    }
}