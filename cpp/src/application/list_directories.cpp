#include "list_directories.h"
#include <filesystem>

namespace nautix::application {

    using namespace nautix::domain;
    namespace fs = std::filesystem;

    std::vector<Directory> ListDirectories::execute(const Directory& root) const {
        std::vector<Directory> result;

        try {
            if (!fs::exists(root.path()) || !fs::is_directory(root.path())) {
                return result; // diretório inválido → retorna vazio
            }

            for (const auto& entry : fs::directory_iterator(root.path())) {
                if (entry.is_directory()) {
                    result.emplace_back(entry.path().string());
                }
            }
        } catch (const fs::filesystem_error&) {
            // Em TDD: ainda não tratamos erros específicos
        }

        return result;
    }

} // namespace nautix::application
