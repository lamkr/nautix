#include "infra/ItemMover.h"

#include <system_error>

#include "common/Logger.h"
#include "infra/errors.h"

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
            constexpr auto copy_options = std::filesystem::copy_options::recursive |
                                      std::filesystem::copy_options::overwrite_existing;

            std::filesystem::copy(sourcePath, targetPath, copy_options, error_code);
            if (error_code) {
                return std::unexpected(error_code); // A cópia falhou
            }

            // A cópia foi bem-sucedida, agora remove o original
            std::filesystem::remove_all(sourcePath, error_code);
            if (error_code) {
                Logger::get()->critical("Move succeeded but source cleanup failed for {}: {}-{}"
                    , sourcePath.string().c_str()
                    , error_code.value()
                    , error_code.message()
                    );
                return std::unexpected(make_error_code(nautix_error::move_cleanup_failed, error_code));
            }

            return true;
        }

        // 4. Se falhou por qualquer outro motivo (permissão, etc.), retorna o erro original
        return std::unexpected(error_code);
    }
}
