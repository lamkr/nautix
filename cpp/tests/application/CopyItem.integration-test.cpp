#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>
#include <catch2/catch_all.hpp>

#include "TempDir.h"
#include "application/CopyItem.h"
#include "infra/ItemCopier.h"

using namespace std::chrono_literals;
using namespace nautix::application;

TEST_CASE("CopyItem use case [integration test]", "[application][use_case]") {

    SECTION("Copy a file to another directory") {
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

    SECTION("Copy a file with new name to same directory") {
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

    SECTION("Copy a directory to another directory") {
        const TempDir tmp;

        // Create subdirectories
        const auto sourcePath = tmp.path / "subdir1";
        const auto targetPath = tmp.path / "subdir2";
        std::filesystem::create_directory(sourcePath);

        const auto filename = "file.txt";
        std::ofstream(sourcePath / filename).put('a');

        auto copier = std::make_shared<nautix::infra::ItemCopier>();
        CopyItem useCase(copier);

        std::filesystem::copy_options options{};
        const auto result = useCase.execute(sourcePath, targetPath, options);

        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE(std::filesystem::exists(targetPath));
        REQUIRE(std::filesystem::exists(targetPath / filename));
    }

}

