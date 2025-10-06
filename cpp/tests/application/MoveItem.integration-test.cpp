#include <filesystem>
#include <fstream>
#include <chrono>
#include <catch2/catch_all.hpp>

#include "TempDir.h"
#include "application/MoveItem.h"
#include "infra/ItemMover.h"

using namespace std::chrono_literals;
using namespace nautix::application;

TEST_CASE("MoveItem use case [integration test]", "[application][use_case]") {

    SECTION("Move a file to another directory") {
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(targetPath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        const auto result = useCase.execute(sourcePath / filename, targetPath / filename);

        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE(std::filesystem::exists(targetPath / filename));
    }

    SECTION("Copy a directory to another directory") {
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        const auto result = useCase.execute(sourcePath, targetPath);

        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE_FALSE(std::filesystem::exists(sourcePath));
        REQUIRE(std::filesystem::exists(targetPath));
        REQUIRE(std::filesystem::exists(targetPath / filename));
    }

}

