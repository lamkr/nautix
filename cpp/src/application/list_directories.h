#pragma once
#include <vector>
#include "../domain/directory.h"

namespace nautix::application {

    class ListDirectories {
    public:
        // Executa o caso de uso a partir de um diretório origem
        std::vector<domain::Directory> execute(
            const domain::Directory& root) const;
    };

} // namespace nautix::application
