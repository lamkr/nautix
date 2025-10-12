#include "application/DeleteItem.h"

#include <expected>

namespace nautix::application {
    std::expected<void, std::error_code> DeleteItem::execute(const std::filesystem::path& path) const {
        return deleter_->deleteItem(path);
    }

}
