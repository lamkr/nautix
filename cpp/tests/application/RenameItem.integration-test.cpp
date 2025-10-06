#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>
#include <catch2/catch_all.hpp>

#include "TempDir.h"
#include "application/RenameItem.h"
#include "infra/ItemRenamer.h"

using namespace std::chrono_literals;
using namespace nautix::application;

TEST_CASE("RenameItem use case [integration test]", "[application][use_case]") {

    SECTION("Rename a existing directory") {
        const TempDir tmp;

        // Create subdirectories
        const auto oldPath = tmp.path / "subdir1";
        const auto newPath = tmp.path / "subdir2";
        std::filesystem::create_directory(oldPath);

        auto renamer = std::make_shared<nautix::infra::ItemRenamer>();
        RenameItem useCase(renamer);

        const auto result = useCase.execute(oldPath, newPath);

        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE_FALSE(std::filesystem::exists(oldPath));
        REQUIRE(std::filesystem::exists(newPath));
    }

    SECTION("Rename a existing file") {
        const TempDir tmp;

        const auto oldPath = tmp.path / "old_file.txt";
        const auto newPath = tmp.path / "new_file.txt";

        // Create a file (should not appear in the directory listing)
        std::ofstream(oldPath).put('a');

        auto renamer = std::make_shared<nautix::infra::ItemRenamer>();
        RenameItem useCase(renamer);

        const auto result = useCase.execute(oldPath, newPath);

        REQUIRE(result.has_value());
        REQUIRE(result.value());
        REQUIRE_FALSE(std::filesystem::exists(oldPath));
        REQUIRE(std::filesystem::exists(newPath));
    }
}

