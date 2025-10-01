#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/directory.h"
#include "application/list_directories.h"

#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

#include "../infra/FakeFileSystemProvider.h"
#include "core/fs.h"
#include "core/time.h"
#include "infra/system_metadata_provider.h"

namespace fs = std::filesystem;
namespace domain = nautix::domain;
namespace app = nautix::application;
namespace infra = nautix::infra;

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

//FakeFileSystemProvider provider;
infra::SystemMetadataProvider provider;

TEST_CASE("ListDirectories sorts by modification time using fake provider", "[unit]") {
    const TempDir tmp;
    const auto creation_time = now();
    provider.create_directory({  // NOLINT
        tmp.path / "d1",
        "d1",
        0,
        1,
        creation_time,
        creation_time,
        creation_time});
    provider.create_directory({   // NOLINT
        tmp.path / "d2", "d2",
        0,
        1,
        creation_time,
        creation_time,
        creation_time});
    const app::ListDirectories usecase(provider);
    const auto result = usecase.execute(tmp.path, app::SortOrder::ByModificationDate);
    REQUIRE(result->size() == 2);
    REQUIRE(result.value()[0].path().filename() == "d1");
}

TEST_CASE("List subdirectories of a directory") {
    const TempDir tmp;

    // Create subdirectories
    const auto sub1 = tmp.path / "subdir1";
    const auto sub2 = tmp.path / "subdir2";
    fs::create_directory(sub1);
    fs::create_directory(sub2);

    // Create a file (should not appear in the directory listing)
    std::ofstream(tmp.path / "arquivo.txt").put('a');

    const app::ListDirectories useCase(provider);

    const auto result = useCase.execute(tmp.path);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 2);
    REQUIRE((result.value()[0].path() == sub1.string() || result.value()[0].path() == sub2.string()));
    REQUIRE((result.value()[1].path() == sub1.string() || result.value()[1].path() == sub2.string()));
}

TEST_CASE("Directory without subdirectories should return empty list") {
    const TempDir tmp;
    const app::ListDirectories useCase(provider);

    const auto result = useCase.execute(tmp.path.string());

    REQUIRE(result.has_value());
    REQUIRE(result->empty());
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

    const app::ListDirectories useCase(provider);

    const auto result = useCase.execute(tmp.path, app::SortOrder::ByName);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 4);
    REQUIRE(result.value()[0].path() == sub1.string());
    REQUIRE(result.value()[1].path() == sub2.string());
    REQUIRE(result.value()[2].path() == sub3.string());
    REQUIRE(result.value()[3].path() == sub4.string());
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

    const app::ListDirectories useCase(provider);

    const auto result = useCase.execute(tmp.path, app::SortOrder::ByCreationDate);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 4);
    REQUIRE(result.value()[0].path() == sub2.string());
    REQUIRE(result.value()[1].path() == sub1.string());
    REQUIRE(result.value()[2].path() == sub4.string());
    REQUIRE(result.value()[3].path() == sub3.string());
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

    const app::ListDirectories useCase(provider);

    const auto result = useCase.execute(tmp.path, app::SortOrder::ByModificationDate);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 4);
    REQUIRE(result.value()[0].path() == sub2.string());
    REQUIRE(result.value()[1].path() == sub1.string());
    REQUIRE(result.value()[2].path() == sub4.string());
    REQUIRE(result.value()[3].path() == sub3.string());
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

    const app::ListDirectories useCase(provider);

    const auto result = useCase.execute(tmp.path, app::SortOrder::ByAccessDate);

    REQUIRE(result.has_value());
    REQUIRE(result->size() == 4);
    REQUIRE(result.value()[0].path() == sub2.string());
    REQUIRE(result.value()[1].path() == sub1.string());
    REQUIRE(result.value()[2].path() == sub4.string());
    REQUIRE(result.value()[3].path() == sub3.string());
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

    fs::create_directory(sub1);
    fs::create_directory(sub4);
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
