#include "infra/ItemsDeleter.h"
#include <system_error>

namespace nautix::infra {
    void ItemsDeleter::deleteItems(const std::vector<std::filesystem::path>& paths) {
        for (const auto& path : paths) {
            std::error_code ec;
            // std::filesystem::remove_all works for files and directories recursively
            if (!std::filesystem::remove_all(path, ec)) {
                // Se ec for setado, houve um erro.
                if (ec) {
                    throw application::ports::DeletionFailure(
                        "Failed to delete " + path.string() + ": " + ec.message()
                    );
                }
            }
        }
    }
}
