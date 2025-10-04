#include "DeleteItems.h"

namespace nautix::application {

    void DeleteItems::execute(const std::vector<std::filesystem::path>& paths) const {
        if (paths.empty()) {
            return; // Nothing to do
        }
        deleter_->deleteItems(paths);
    }

}