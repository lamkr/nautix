#include "application/ListDirectories.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <vector>
#include <expected>
#include <filesystem>
#include <iostream>

#include "domain/directory.h"

// Mock da nossa interface
class MockDirectoriesLister final : public nautix::application::IDirectoriesLister {
public:
    // --- CORREÇÃO AQUI ---
    // 1. Criamos um alias para o tipo de retorno que contém vírgula.
    using ListResult = std::expected<std::vector<nautix::domain::Directory>, std::error_code>;

    // 2. A macro agora usa o alias 'ListResult', que não tem vírgulas.
    MAKE_CONST_MOCK2(list_directories,
                     ListResult(const std::filesystem::path&, nautix::application::SortOrder));
};

TEST_CASE("ListDirectories use case", "[application][use_case]") {
    using namespace nautix::application;
    using namespace nautix::domain;

    auto mock_lister = std::make_shared<MockDirectoriesLister>();
    auto use_case = std::make_unique<ListDirectories>(mock_lister);

    const std::filesystem::path target_path = "/home/user";
    constexpr auto sort_order = SortOrder::ByName;

    SECTION("should get list of directories") {
        // Arrange
        std::vector<Directory> expected_dirs;
        expected_dirs.emplace_back(Directory(target_path / "Downloads", "Downloads", 4096, {}, {}, {}, {}));
        expected_dirs.emplace_back(Directory(target_path / "Pictures", "Pictures", 4096, {}, {}, {}, {}));

        // Expect
        REQUIRE_CALL(*mock_lister, list_directories(target_path, sort_order)).RETURN(expected_dirs);

        // Act
        const auto result = use_case->execute(target_path, sort_order);

        // Assert
        REQUIRE(result.has_value());
        REQUIRE(result.value().size() == 2);
        REQUIRE(result.value()[0].name() == "Downloads");
    }

    SECTION("should fail if path not found") {
        // Arrange
        const auto expected_error = std::make_error_code(std::errc::no_such_file_or_directory);

        // Expect
        REQUIRE_CALL(*mock_lister, list_directories(target_path, sort_order))
            .RETURN(std::unexpected(expected_error));

        // Act
        const auto result = use_case->execute(target_path, sort_order);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should fail if path is not a directory") {
        const auto expected_error = std::make_error_code(std::errc::not_a_directory);
        // Arrange
        std::vector<Directory> expected_dirs;

        // Expect
        REQUIRE_CALL(*mock_lister, list_directories(target_path, sort_order))
            .RETURN(std::unexpected(expected_error));

        // Act
        const auto result = use_case->execute(target_path, sort_order);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should get list sorted by owner") {
        // Arrange
        std::vector<Directory> expected_dirs;
        const auto sub2 = "mno";
        const auto sub1 = "abc";
        const auto sub4 = "xyw";
        const auto sub3 = "stu";
        expected_dirs.emplace_back(Directory(target_path / sub4, sub4, 4096, Owner{1,"user1"}, {}, {}, {}));
        expected_dirs.emplace_back(Directory(target_path / sub3, sub3, 4096, Owner{2,"user2"}, {}, {}, {}));
        expected_dirs.emplace_back(Directory(target_path / sub2, sub2, 4096, Owner{3,"user3"}, {}, {}, {}));
        expected_dirs.emplace_back(Directory(target_path / sub1, sub1, 4096, Owner{4,"user4"}, {}, {}, {}));

        // Expect
        REQUIRE_CALL(*mock_lister, list_directories(target_path, SortOrder::ByOwner)).RETURN(expected_dirs);

        // Act
        const auto result = use_case->execute(target_path, SortOrder::ByOwner);

        // Assert
        REQUIRE(result.has_value());
        REQUIRE(result.value().size() == 4);
        REQUIRE(result.value()[0].name() == sub4);
        REQUIRE(result.value()[1].name() == sub3);
        REQUIRE(result.value()[2].name() == sub2);
        REQUIRE(result.value()[3].name() == sub1);
    }

}