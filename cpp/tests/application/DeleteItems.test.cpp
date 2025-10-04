#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp> // Supondo o uso do trompeloeil para mocks com Catch2

#include "application/DeleteItems.h"
#include "application/ports/IItemsDeleter.h"

// Mock da nossa interface
class MockItemsDeleter final : public nautix::application::ports::IItemsDeleter {
public:
    MAKE_MOCK1(deleteItems, void(const std::vector<std::filesystem::path>&));
};

TEST_CASE("DeleteItems use case", "[application][use_case]") {
    
    SECTION("should call the infrastructure port with the correct paths") {
        // Arrange
        auto mockDeleter = std::make_shared<MockItemsDeleter>();
        const auto deleteItemsUseCase = std::make_unique<nautix::application::DeleteItems>(mockDeleter);

        const std::vector<std::filesystem::path> pathsToDelete = {
            "/tmp/file1.txt",
            "/tmp/dir1"
        };
        
        // Expect
        REQUIRE_CALL(*mockDeleter, deleteItems(pathsToDelete))
            .TIMES(1);

        // Act
        deleteItemsUseCase->execute(pathsToDelete);
    }
}