#define CATCH_CONFIG_MAIN
#include <filesystem>
#include <fstream>
#include <catch2/catch_all.hpp>

#include "../../src/application/list_directories.h"
#include "../../src/infra/directories_lister.h"

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

TEST_CASE("List directories") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub1 = tmp.path / "subdir1";
    const auto sub2 = tmp.path / "subdir2";
    std::filesystem::create_directory(sub1);
    std::filesystem::create_directory(sub2);

    // Create a file (should not appear in the directory listing)
    std::ofstream(tmp.path / "arquivo.txt").put('a');

    auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
    nautix::application::ListDirectories useCase(lister);

    const auto result = useCase.execute(tmp.path);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 2);
    REQUIRE((result.value()[0].path() == sub1.string() || result.value()[0].path() == sub2.string()));
    REQUIRE((result.value()[1].path() == sub1.string() || result.value()[1].path() == sub2.string()));
}