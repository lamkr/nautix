#include "infra/ItemMover.h"

#include <system_error>

#include "common/Logger.h"
#include "infra/errors.h"

namespace nautix::infra {
    std::expected<int, std::error_code>
            ItemMover::moveItems(
                const std::vector<std::filesystem::path>& sourcePaths,
                const std::filesystem::path& targetPath)
    {
        int itemsMovedSuccessfully = 0;
        std::error_code error_code;

        // Garante que o diretório de destino existe.
        if (!std::filesystem::is_directory(targetPath, error_code)) {
            Logger::get()->error("Move target is not a directory: '{}'", targetPath.string());
            return std::unexpected(std::make_error_code(std::errc::not_a_directory));
        }

        // Itera sobre cada caminho de origem.
        for (const auto& sourcePath : sourcePaths) {
            // Ignora itens que não existem mais (podem ter sido movidos como parte de um diretório pai).
            if (!std::filesystem::exists(sourcePath)) {
                continue;
            }

            // Constrói o caminho de destino final.
            const auto finalTargetPath = targetPath / sourcePath.filename();
            error_code.clear(); // Limpa o código de erro antes da próxima operação.

            // 1. Tenta a operação rápida e atômica 'rename' primeiro.
            std::filesystem::rename(sourcePath, finalTargetPath, error_code);

            if (!error_code) {
                // Sucesso!
                itemsMovedSuccessfully++;
                continue;
            }

            // 2. Se falhou, verifica se foi um erro de "cross-device".
            if (error_code == std::errc::cross_device_link) {
                Logger::get()->info("Cross-device move detected for '{}'. Falling back to copy+delete.", sourcePath.string());

                // 3. Se sim, inicia o fallback: Cópia + Exclusão.
                constexpr auto copy_options = std::filesystem::copy_options::recursive;
                //constexpr auto copy_options = std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing;

                std::filesystem::copy(sourcePath, finalTargetPath, copy_options, error_code);

                if (error_code) {
                    Logger::get()->error("Fallback copy failed for '{}': {}", sourcePath.string(), error_code.message());
                    // Retorna o erro da cópia e o número de itens movidos até agora.
                    return std::unexpected(error_code);
                }

                // A cópia foi bem-sucedida, agora remove o original.
                std::filesystem::remove_all(sourcePath, error_code);
                if (error_code) {
                    Logger::get()->critical("Move succeeded but source cleanup failed for {}: {}-{}"
                        , sourcePath.string().c_str()
                        , error_code.value()
                        , error_code.message()
                        );
                    // A operação foi "bem-sucedida" do ponto de vista do usuário, mas o erro de limpeza é grave.
                    // Paramos aqui e retornamos o erro para a UI.
                    return std::unexpected(make_error_code(nautix_error::move_cleanup_failed, error_code));
                }

                itemsMovedSuccessfully++;

            } else {
                // 4. Se falhou por qualquer outro motivo (permissão, etc.), para tudo e retorna o erro.
                Logger::get()->error("Move failed for '{}': {}", sourcePath.string(), error_code.message());
                return std::unexpected(error_code);
            }
        }

        return itemsMovedSuccessfully;
    }
}
