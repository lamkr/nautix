#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/directory.h"
#include "application/list_directories.h"

#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

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
    const TempDir tmp;

    // Create subdirectories
    const auto sub1 = tmp.path / "subdir1";
    const auto sub2 = tmp.path / "subdir2";
    std::filesystem::create_directory(sub1);
    std::filesystem::create_directory(sub2);

    // Create a file (should not appear in the directory listing)
    std::ofstream(tmp.path / "arquivo.txt").put('a');

    const Directory dir(tmp.path.string());
    constexpr ListDirectories useCase;

    const auto result = useCase.execute(dir);

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

TEST_CASE("List directories sorted by name") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    std::filesystem::create_directory(sub1);
    std::filesystem::create_directory(sub2);
    std::filesystem::create_directory(sub3);
    std::filesystem::create_directory(sub4);

    const Directory dir(tmp.path);
    constexpr ListDirectories useCase;

    const auto result = useCase.execute(dir, SortOrder::ByName);

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

    std::filesystem::create_directory(sub2);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub1);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub4);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub3);

    const Directory dir(tmp.path);
    constexpr ListDirectories useCase;

    const auto result = useCase.execute(dir, SortOrder::ByCreationDate);

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

    std::filesystem::create_directory(sub2);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub1);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub4);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub3);

    const Directory dir(tmp.path);
    constexpr ListDirectories useCase;

    const auto result = useCase.execute(dir, SortOrder::ByModificationDate);

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

    std::filesystem::create_directory(sub2);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub1);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub4);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::filesystem::create_directory(sub3);

    const Directory dir(tmp.path);
    constexpr ListDirectories useCase;

    const auto result = useCase.execute(dir, SortOrder::ByAccessDate);

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

    std::filesystem::create_directory(sub1);
    std::filesystem::create_directory(sub4);
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

    std::filesystem::create_directory(sub1);
    std::filesystem::create_directory(sub4);
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