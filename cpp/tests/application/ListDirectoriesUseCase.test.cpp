#include "application/ListDirectories.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <vector>
#include <expected>
#include <filesystem>
#include "domain/directory.h"


// Mock da nossa interface
class MockDirectoriesLister : public nautix::application::IDirectoriesLister {
public:
    // --- CORREÇÃO AQUI ---
    // 1. Criamos um alias para o tipo de retorno que contém vírgula.
    using ListResult = std::expected<std::vector<nautix::domain::Directory>, std::error_code>;

    // 2. A macro agora usa o alias 'ListResult', que não tem vírgulas.
    MAKE_CONST_MOCK2(list_directories,
                     ListResult(const std::filesystem::path&, nautix::application::SortOrder));
};

TEST_CASE("ListDirectories Use Case", "[application][use_case]") {
    using namespace nautix::application;
    using namespace nautix::domain;

    auto mock_lister = std::make_shared<MockDirectoriesLister>();
    auto use_case = std::make_unique<ListDirectories>(mock_lister);

    const std::filesystem::path target_path = "/home/user";
    const auto sort_order = SortOrder::ByName;

    SECTION("should successfully return a list of directories") {
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

    SECTION("should return an error if the lister fails") {
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
}