#include "infra/ItemDeleter.h"
#include <system_error>

namespace nautix::infra {
    std::expected<void, std::error_code> ItemDeleter::deleteItem(const std::filesystem::path& path) {
        std::error_code error_code;
        std::filesystem::remove(path, error_code);
        if (error_code) {
            return std::unexpected(error_code);
        }
        return {};
    }
}
