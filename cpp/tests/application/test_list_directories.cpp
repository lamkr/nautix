#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/directory.h"
#include "application/list_directories.h"

#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;
namespace domain = nautix::domain;
namespace app = nautix::application;

// Helper to create temporary test environment.
struct TempDir {
    fs::path path;

    TempDir() {
        path = fs::temp_directory_path() /
               fs::path("nautix_test_" + std::to_string(::getpid()));
        fs::create_directory(path);
    }

    ~TempDir() {
        fs::remove_all(path);
    }
};

TEST_CASE("List subdirectories of a directory") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub1 = tmp.path / "subdir1";
    const auto sub2 = tmp.path / "subdir2";
    fs::create_directory(sub1);
    fs::create_directory(sub2);

    // Create a file (should not appear in the directory listing)
    std::ofstream(tmp.path / "arquivo.txt").put('a');

    constexpr app::ListDirectories useCase;

    const auto result = useCase.execute(tmp.path);

    REQUIRE(result.size() == 2);
    REQUIRE((result[0].path() == sub1.string() || result[0].path() == sub2.string()));
    REQUIRE((result[1].path() == sub1.string() || result[1].path() == sub2.string()));
}

TEST_CASE("Directory without subdirectories should return empty list") {
    const TempDir tmp;
    constexpr app::ListDirectories useCase;
    const auto result = useCase.execute(tmp.path.string());

    REQUIRE(result.empty());
}

TEST_CASE("List directories sorted by name") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    fs::create_directory(sub1);
    fs::create_directory(sub2);
    fs::create_directory(sub3);
    fs::create_directory(sub4);

    constexpr app::ListDirectories useCase;

    const auto result = useCase.execute(tmp.path, app::SortOrder::ByName);

    REQUIRE(result.size() == 4);
    REQUIRE(result[0].path() == sub1.string());
    REQUIRE(result[1].path() == sub2.string());
    REQUIRE(result[2].path() == sub3.string());
    REQUIRE(result[3].path() == sub4.string());
}

TEST_CASE("List directories sorted by creation date") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    fs::create_directory(sub2);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub1);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub4);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub3);

    constexpr app::ListDirectories useCase;

    const auto result = useCase.execute(tmp.path, app::SortOrder::ByCreationDate);

    REQUIRE(result.size() == 4);
    REQUIRE(result[0].path() == sub2.string());
    REQUIRE(result[1].path() == sub1.string());
    REQUIRE(result[2].path() == sub4.string());
    REQUIRE(result[3].path() == sub3.string());
}

TEST_CASE("List directories sorted by modification date") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    fs::create_directory(sub2);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub1);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub4);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub3);

    constexpr app::ListDirectories useCase;

    const auto result = useCase.execute(tmp.path, app::SortOrder::ByModificationDate);

    REQUIRE(result.size() == 4);
    REQUIRE(result[0].path() == sub2.string());
    REQUIRE(result[1].path() == sub1.string());
    REQUIRE(result[2].path() == sub4.string());
    REQUIRE(result[3].path() == sub3.string());
}

TEST_CASE("List directories sorted by access date") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    fs::create_directory(sub2);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub1);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub4);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    fs::create_directory(sub3);

    constexpr app::ListDirectories useCase;

    const auto result = useCase.execute(tmp.path, app::SortOrder::ByAccessDate);

    REQUIRE(result.size() == 4);
    REQUIRE(result[0].path() == sub2.string());
    REQUIRE(result[1].path() == sub1.string());
    REQUIRE(result[2].path() == sub4.string());
    REQUIRE(result[3].path() == sub3.string());
}

/*
 * TODO criar com owner diferentes
TEST_CASE("List directories sorted by owner") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";

    fs::create_directory(sub1);
    fs::create_directory(sub4);
    const Directory dir(tmp.path);
    constexpr ListDirectories useCase;

    const auto result = useCase.execute(dir, SortOrder::ByAccessDate);

    REQUIRE(result.size() == 4);
    REQUIRE(result[0].path() == sub2.string());
    REQUIRE(result[1].path() == sub1.string());
    REQUIRE(result[2].path() == sub4.string());
    REQUIRE(result[3].path() == sub3.string());
}

 *TODO falta criar arquivos com tamanhos diferentes dentro de cada dubdr

 *TEST_CASE("List directories sorted by size") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";

    fs::create_directory(sub1);
    fs::create_directory(sub4);
    const Directory dir(tmp.path);
    constexpr ListDirectories useCase;

    const auto result = useCase.execute(dir, SortOrder::ByAccessDate);

    REQUIRE(result.size() == 4);
    REQUIRE(result[0].path() == sub2.string());
    REQUIRE(result[1].path() == sub1.string());
    REQUIRE(result[2].path() == sub4.string());
    REQUIRE(result[3].path() == sub3.string());
}
*/