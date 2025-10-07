#include <filesystem>
#include <fstream>
#include <chrono>
#include <iostream>
#include <thread>
#include <catch2/catch_all.hpp>

#include "TempDir.h"
#include "application/ListDirectories.h"
#include "infra/DirectoriesLister.h"

using namespace std::chrono_literals;
using namespace nautix::application;

TEST_CASE("ListDirectories use case [integration test]", "[application][use_case]") {

    std::chrono::milliseconds delay = 150ms;
    
    SECTION("Return a list of directories") {
        const TempDir tmp;

        // Create subdirectories
        const auto sub1 = tmp.path / "subdir1";
        const auto sub2 = tmp.path / "subdir2";
        std::filesystem::create_directory(sub1);
        std::filesystem::create_directory(sub2);

        // Create a file (should not appear in the directory listing)
        std::ofstream(tmp.path / "arquivo.txt").put('a');

        auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path);

        REQUIRE(result.has_value());
        REQUIRE(result->size() == 2);
        REQUIRE((result.value()[0].path() == sub1.string() || result.value()[0].path() == sub2.string()));
        REQUIRE((result.value()[1].path() == sub1.string() || result.value()[1].path() == sub2.string()));
    }

    SECTION("Directory without subdirectories should return empty list") {
        const TempDir tmp;
        const auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path.string());

        REQUIRE(result.has_value());
        REQUIRE(result->empty());
    }

    SECTION("Return error if path not found") {
        const TempDir tmp;

        // Expect
        auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path / "this_folder_does_not_exist" );

        const auto expected_error = std::make_error_code(std::errc::no_such_file_or_directory);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("Return error if path is not a directory") {
        const TempDir tmp;

        // Create a file (should not appear in the directory listing)
        std::string filename = "arquivo.txt";
        std::ofstream(tmp.path / filename).put('a');

        auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path / filename);

        const auto expected_error = std::make_error_code(std::errc::not_a_directory);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("Return error if current user have forbidden access to path") {
        auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute("/root");

        const auto expected_error = std::make_error_code(std::errc::permission_denied);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("Sorted by name") {
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
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path, SortOrder::ByName);

        REQUIRE(result.has_value());
        REQUIRE(result->size() == 4);
        REQUIRE(result.value()[0].path() == sub1.string());
        REQUIRE(result.value()[1].path() == sub2.string());
        REQUIRE(result.value()[2].path() == sub3.string());
        REQUIRE(result.value()[3].path() == sub4.string());
    }

    SECTION("Sorted by creation date") {
        const TempDir tmp;

        // Create subdirectories
        const auto sub2 = tmp.path / "mno";
        const auto sub1 = tmp.path / "abc";
        const auto sub4 = tmp.path / "xyw";
        const auto sub3 = tmp.path / "stu";

        std::filesystem::create_directory(sub4);
        std::this_thread::sleep_for(150ms);
        std::filesystem::create_directory(sub3);
        std::this_thread::sleep_for(150ms);
        std::filesystem::create_directory(sub2);
        std::this_thread::sleep_for(150ms);
        std::filesystem::create_directory(sub1);

        auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path, SortOrder::ByCreationDate);

        REQUIRE(result.has_value());
        REQUIRE(result->size() == 4);
        REQUIRE(result.value()[0].path() == sub4.string());
        REQUIRE(result.value()[1].path() == sub3.string());
        REQUIRE(result.value()[2].path() == sub2.string());
        REQUIRE(result.value()[3].path() == sub1.string());
    }

    SECTION("Sorted by modification date") {
        const TempDir tmp;

        // Create subdirectories
        const auto sub2 = tmp.path / "mno";
        const auto sub1 = tmp.path / "abc";
        const auto sub4 = tmp.path / "xyw";
        const auto sub3 = tmp.path / "stu";

        std::filesystem::create_directory(sub2);
        std::this_thread::sleep_for(delay);
        std::filesystem::create_directory(sub4);
        std::this_thread::sleep_for(delay);
        std::filesystem::create_directory(sub1);
        std::this_thread::sleep_for(delay);
        std::filesystem::create_directory(sub3);

        auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path, SortOrder::ByModificationDate);

        REQUIRE(result.has_value());
        REQUIRE(result->size() == 4);
        REQUIRE(result.value()[0].path() == sub2.string());
        REQUIRE(result.value()[1].path() == sub4.string());
        REQUIRE(result.value()[2].path() == sub1.string());
        REQUIRE(result.value()[3].path() == sub3.string());
    }

    SECTION("Sorted by access date") {
        const TempDir tmp;

        // Create subdirectories
        const auto sub2 = tmp.path / "mno";
        const auto sub1 = tmp.path / "abc";
        const auto sub4 = tmp.path / "xyw";
        const auto sub3 = tmp.path / "stu";

        std::filesystem::create_directory(sub1);
        std::this_thread::sleep_for(delay);
        std::filesystem::create_directory(sub3);
        std::this_thread::sleep_for(delay);
        std::filesystem::create_directory(sub2);
        std::this_thread::sleep_for(delay);
        std::filesystem::create_directory(sub4);

        auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path, SortOrder::ByAccessDate);

        REQUIRE(result.has_value());
        REQUIRE(result->size() == 4);
        REQUIRE(result.value()[0].path() == sub1.string());
        REQUIRE(result.value()[1].path() == sub3.string());
        REQUIRE(result.value()[2].path() == sub2.string());
        REQUIRE(result.value()[3].path() == sub4.string());
    }

    SECTION("List directories sorted by size") {
        const TempDir tmp;

        // Create subdirectories
        const auto sub2 = tmp.path / "mno";
        const auto sub1 = tmp.path / "abc";
        const auto sub4 = tmp.path / "xyw";
        const auto sub3 = tmp.path / "stu";

        std::filesystem::create_directory(sub2);
        std::ofstream(sub2 / "file_0.txt").put('a');
        std::ofstream(sub2 / "file_1.txt").put('a');

        std::filesystem::create_directory(sub1);
        std::ofstream(sub1 / "file_2.txt").put('a');
        std::ofstream(sub1 / "file_3.txt").put('a');

        std::filesystem::create_directory(sub4);
        std::ofstream(sub4 / "file_4.txt").put('a');

        std::filesystem::create_directory(sub3);
        std::ofstream(sub3 / "file_5.txt").put('a');
        std::ofstream(sub3 / "file_6.txt").put('a');
        std::ofstream(sub3 / "file_7.txt").put('a');
        std::ofstream(sub3 / "file_8.txt").put('a');
        std::ofstream(sub3 / "file_9.txt").put('a');

        auto lister = std::make_shared<nautix::infra::DirectoriesLister>();
        ListDirectories useCase(lister);

        const auto result = useCase.execute(tmp.path, SortOrder::BySize);

        REQUIRE(result.has_value());
        REQUIRE(result->size() == 4);

        REQUIRE(result.value()[0].path() == sub4.string());
        REQUIRE(result.value()[1].path() == sub1.string());
        REQUIRE(result.value()[2].path() == sub2.string());
        REQUIRE(result.value()[3].path() == sub3.string());
    }

}

