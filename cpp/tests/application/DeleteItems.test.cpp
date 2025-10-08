#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>

#include "application/DeleteItem.h"
#include "application/ports/IItemDeleter.h"

class MockItemDeleter final : public nautix::application::ports::IItemDeleter {
public:
    using DeleteResult = std::expected<void, std::error_code>;
    MAKE_MOCK1(deleteItem, DeleteResult(const std::filesystem::path&));
};

TEST_CASE("DeleteItem use case", "[application][use_case]") {
    using namespace nautix::application;
    auto mockDeleter = std::make_shared<MockItemDeleter>();
    DeleteItem useCase(mockDeleter);
    std::filesystem::path item_path = "/tmp/file1.txt";
    constexpr std::expected<void, std::error_code> success{};

    SECTION("should rename a file") {
        // Expect
        // O caso de uso deve chamar 'deleteItem' no port injetado.
        REQUIRE_CALL(*mockDeleter, deleteItem(item_path))
                    .RETURN(success); // Retorna sucesso (vazio)

        // Act
        auto result = useCase.execute(item_path);

        // Assert
        REQUIRE(result.has_value());
    }

    SECTION("should return an error when deletion fails") {
        // Arrange
        const auto expected_error = std::make_error_code(std::errc::permission_denied);

        // Expect
        REQUIRE_CALL(*mockDeleter, deleteItem(item_path))
                    .RETURN(std::unexpected(expected_error));

        // Act
        auto result = useCase.execute(item_path);

        // Assert
        REQUIRE_FALSE(result.has_value());
        REQUIRE(result.error() == expected_error);
    }

    SECTION("should delete a directory") {
        // Arrange
        item_path = "/tmp/dir";

        // Expert
        REQUIRE_CALL(*mockDeleter, deleteItem(item_path))
                    .RETURN(success);

        // Act (Agir)
        auto result = mockDeleter->deleteItem(item_path);

        // Assert (Verificar)
        REQUIRE(result.has_value());
    }
}