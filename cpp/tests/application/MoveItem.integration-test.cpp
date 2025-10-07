#include <filesystem>
#include <fstream>
#include <chrono>
#include <memory>
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

        const auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        const auto result = useCase.execute({sourcePath / filename}, targetPath );

        REQUIRE(result.has_value());
        REQUIRE(result.value() == 1);
        REQUIRE_FALSE(std::filesystem::exists(sourcePath / filename));
        REQUIRE(std::filesystem::exists(targetPath / filename));
    }

    SECTION("Move a directory to another directory") {
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

        const auto result = useCase.execute({sourcePath}, targetPath);

        REQUIRE(result.has_value());
        REQUIRE(result.value() == 1);
        REQUIRE_FALSE(std::filesystem::exists(sourcePath));
        std::filesystem::path targetPathComplete = targetPath / sourcePath.filename();
        REQUIRE(std::filesystem::exists(targetPathComplete));
        targetPathComplete /= filename;
        REQUIRE(std::filesystem::exists(targetPathComplete));
    }

    SECTION("Move multiples files to another directory") {
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(targetPath);

        const auto filename1 = "file1.txt";
        const auto filename2 = "file2.txt";
        const auto filename3 = "file3.txt";

        std::vector<std::filesystem::path> sourcePaths;
        sourcePaths.emplace_back(sourcePath / filename1);
        sourcePaths.emplace_back(sourcePath / filename2);
        sourcePaths.emplace_back(sourcePath / filename3);

        std::ofstream(sourcePaths[0]).put('a');
        std::ofstream(sourcePaths[1]).put('a');
        std::ofstream(sourcePaths[2]).put('a');

        auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        const auto result = useCase.execute(sourcePaths, targetPath);

        REQUIRE(result.has_value());
        REQUIRE(result.value() == 3);
        REQUIRE_FALSE(std::filesystem::exists(sourcePaths[0]));
        REQUIRE_FALSE(std::filesystem::exists(sourcePaths[1]));
        REQUIRE_FALSE(std::filesystem::exists(sourcePaths[2]));
        REQUIRE(std::filesystem::exists(targetPath / filename1));
        REQUIRE(std::filesystem::exists(targetPath / filename2));
        REQUIRE(std::filesystem::exists(targetPath / filename3));
    }

    SECTION("Move multiples files and directories to another directory") {
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";

        const auto subdir3 = "subdir3";
        const auto subdir4 = "subdir4";

        std::filesystem::create_directory(sourcePath);
        std::filesystem::create_directory(sourcePath / subdir3 );
        std::filesystem::create_directory(sourcePath / subdir4 );

        const auto filename1 = "file1.txt";
        const auto filename2 = "file2.txt";
        const auto filename3 = "file3.txt";

        std::vector<std::filesystem::path> sourcePaths;
        sourcePaths.emplace_back(sourcePath / filename1);
        sourcePaths.emplace_back(sourcePath / filename2);
        sourcePaths.emplace_back(sourcePath / filename3);

        std::ofstream(sourcePaths[0]).put('a');
        std::ofstream(sourcePaths[1]).put('a');
        std::ofstream(sourcePaths[2]).put('a');

        sourcePaths.emplace_back(sourcePath / subdir3);
        sourcePaths.emplace_back(sourcePath / subdir4);

        std::filesystem::create_directory(targetPath);

        auto mover = std::make_shared<nautix::infra::ItemMover>();
        MoveItem useCase(mover);

        const auto result = useCase.execute(sourcePaths, targetPath);

        REQUIRE(result.has_value());
        REQUIRE(result.value() == 5);
        REQUIRE_FALSE(std::filesystem::exists(sourcePaths[0]));
        REQUIRE_FALSE(std::filesystem::exists(sourcePaths[1]));
        REQUIRE_FALSE(std::filesystem::exists(sourcePaths[2]));
        REQUIRE_FALSE(std::filesystem::exists(sourcePaths[3]));
        REQUIRE_FALSE(std::filesystem::exists(sourcePaths[4]));
        REQUIRE(std::filesystem::exists(targetPath / subdir3));
        REQUIRE(std::filesystem::exists(targetPath / subdir4));
        REQUIRE(std::filesystem::exists(targetPath / filename1));
        REQUIRE(std::filesystem::exists(targetPath / filename2));
        REQUIRE(std::filesystem::exists(targetPath / filename3));
    }
}

