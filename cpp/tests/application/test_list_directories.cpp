#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/directory.h"
#include "application/list_directories.h"

#include <filesystem>
#include <fstream>

using namespace nautix::domain;
using namespace nautix::application;

// Helper to create temporary test environment.
struct TempDir {
    std::filesystem::path path;

    TempDir() {
        path = std::filesystem::temp_directory_path() /
               std::filesystem::path("nautix_test_" + std::to_string(::getpid()));
        std::filesystem::create_directory(path);
    }

    ~TempDir() {
        std::filesystem::remove_all(path);
    }
};

TEST_CASE("List subdirectories of a directory") {
    TempDir tmp;

    // Create subdirectories
    auto sub1 = tmp.path / "subdir1";
    auto sub2 = tmp.path / "subdir2";
    std::filesystem::create_directory(sub1);
    std::filesystem::create_directory(sub2);

    // Create a file (should not appear in the directory listing)
    std::ofstream(tmp.path / "arquivo.txt").put('a');

    Directory dir(tmp.path.string());
    ListDirectories useCase;

    auto result = useCase.execute(dir);

    REQUIRE(result.size() == 2);
    REQUIRE((result[0].path() == sub1.string() || result[0].path() == sub2.string()));
    REQUIRE((result[1].path() == sub1.string() || result[1].path() == sub2.string()));
}

TEST_CASE("Directory without subdirectories should return empty list") {
    const TempDir tmp;

    const Directory root(tmp.path.string());
    constexpr ListDirectories useCase;

    const auto result = useCase.execute(root);

    REQUIRE(result.empty());
}
