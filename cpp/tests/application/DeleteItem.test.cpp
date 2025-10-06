#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>

#include "application/DeleteItem.h"
#include "application/ports/IItemDeleter.h"

class MockItemDeleter final : public nautix::application::ports::IItemDeleter {
public:
    using DeleteResult = std::expected<bool, std::error_code>;
    MAKE_MOCK1(deleteItem, DeleteResult(const std::filesystem::path&));
};

TEST_CASE("DeleteItem use case", "[application][use_case]") {
    auto mockDeleter = std::make_shared<MockItemDeleter>();
    constexpr std::expected<bool, std::error_code> success_result = true;

    SECTION("should rename a file") {
        // Arrange
        const std::filesystem::path item_path = "/tmp/file1.txt";

        // Expert
        REQUIRE_CALL(*mockDeleter, deleteItem(item_path))
                    .RETURN(success_result);

        // Act (Agir)
        auto result = mockDeleter->deleteItem(item_path);

        // Assert (Verificar)
        REQUIRE(result.has_value());
        REQUIRE(result.value() == true);
    }

    SECTION("should rename a directory") {
        // Arrange
        const std::filesystem::path item_path = "/tmp/dir";

        // Expert
        REQUIRE_CALL(*mockDeleter, deleteItem(item_path))
                    .RETURN(success_result);

        // Act (Agir)
        auto result = mockDeleter->deleteItem(item_path);

        // Assert (Verificar)
        REQUIRE(result.has_value());
        REQUIRE(result.value() == true);
    }
}