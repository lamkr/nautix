#define CATCH_CONFIG_MAIN
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>
#include <catch2/catch_all.hpp>

#include "application/ListDirectories.h"
#include "infra/DirectoriesLister.h"

using namespace std::chrono_literals;

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

TEST_CASE("Directory without subdirectories should return empty list") {
    const TempDir tmp;
    const auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
    nautix::application::ListDirectories useCase(lister);

    const auto result = useCase.execute(tmp.path.string());

    REQUIRE(result.has_value());
    REQUIRE(result->empty());
}

TEST_CASE("Sorted by name") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    std::filesystem::create_directory(sub2);
    std::filesystem::create_directory(sub1);
    std::filesystem::create_directory(sub4);
    std::filesystem::create_directory(sub3);

    auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
    nautix::application::ListDirectories useCase(lister);

    const auto result = useCase.execute(tmp.path, SortOrder::ByName);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 4);
    REQUIRE(result.value()[0].path() == sub1.string());
    REQUIRE(result.value()[1].path() == sub2.string());
    REQUIRE(result.value()[2].path() == sub3.string());
    REQUIRE(result.value()[3].path() == sub4.string());
}

TEST_CASE("Sorted by creation date") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    std::filesystem::create_directory(sub4);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub3);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub2);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub1);

    auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
    nautix::application::ListDirectories useCase(lister);

    const auto result = useCase.execute(tmp.path, SortOrder::ByCreationDate);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 4);
    REQUIRE(result.value()[0].path() == sub4.string());
    REQUIRE(result.value()[1].path() == sub3.string());
    REQUIRE(result.value()[2].path() == sub2.string());
    REQUIRE(result.value()[3].path() == sub1.string());
}

TEST_CASE("Sorted by modification date") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    std::filesystem::create_directory(sub2);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub4);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub1);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub3);

    auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
    nautix::application::ListDirectories useCase(lister);

    const auto result = useCase.execute(tmp.path, SortOrder::ByModificationDate);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 4);
    REQUIRE(result.value()[0].path() == sub2.string());
    REQUIRE(result.value()[1].path() == sub4.string());
    REQUIRE(result.value()[2].path() == sub1.string());
    REQUIRE(result.value()[3].path() == sub3.string());
}

TEST_CASE("Sorted by access date") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub2 = tmp.path / "mno";
    const auto sub1 = tmp.path / "abc";
    const auto sub4 = tmp.path / "xyw";
    const auto sub3 = tmp.path / "stu";

    std::filesystem::create_directory(sub1);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub3);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub2);
    std::this_thread::sleep_for(250ms);
    std::filesystem::create_directory(sub4);

    auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
    nautix::application::ListDirectories useCase(lister);

    const auto result = useCase.execute(tmp.path, SortOrder::ByAccessDate);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 4);
    REQUIRE(result.value()[0].path() == sub1.string());
    REQUIRE(result.value()[1].path() == sub3.string());
    REQUIRE(result.value()[2].path() == sub2.string());
    REQUIRE(result.value()[3].path() == sub4.string());
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
    REQUIRE(result.value()[0].path() == sub2.string());
    REQUIRE(result.value()[1].path() == sub1.string());
    REQUIRE(result.value()[2].path() == sub4.string());
    REQUIRE(result.value()[3].path() == sub3.string());
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
    REQUIRE(result.value()[0].path() == sub2.string());
    REQUIRE(result.value()[1].path() == sub1.string());
    REQUIRE(result.value()[2].path() == sub4.string());
    REQUIRE(result.value()[3].path() == sub3.string());
}
*/