#include "infra/ItemMover.h"

#include <system_error>

namespace nautix::infra {
    std::expected<bool, std::error_code>
        ItemMover::moveItem(
            const std::filesystem::path& sourcePath,
            const std::filesystem::path& targetPath
        )
    {
        std::error_code error_code;
        // 1. Tenta a operação rápida e atômica primeiro
        std::filesystem::rename(sourcePath, targetPath, error_code);
        if (!error_code) {
            return true;
        }

        // 2. Se falhou, verifica se foi um erro de "cross-device"
        if (error_code == std::errc::cross_device_link) {
            // 3. Se sim, inicia o fallback: Cópia + Exclusão

            // Opções de cópia para recursividade e tratamento de arquivos existentes
            const auto copy_options = std::filesystem::copy_options::recursive |
                                      std::filesystem::copy_options::overwrite_existing;

            std::filesystem::copy(sourcePath, targetPath, copy_options, error_code);
            if (error_code) {
                return std::unexpected(error_code); // A cópia falhou
            }

            // A cópia foi bem-sucedida, agora remove o original
            std::filesystem::remove_all(sourcePath, error_code);
            if (error_code) {
                // TODO fazer log do erro
                // Este é um estado problemático: a cópia foi feita, mas o original não foi removido.
                // É preciso logar e talvez retornar um código de erro especial.
                return std::unexpected(error_code);
            }

            return true;
        }

        // 4. Se falhou por qualquer outro motivo (permissão, etc.), retorna o erro original
        return std::unexpected(error_code);
    }
}
