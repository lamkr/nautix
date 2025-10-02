#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include "application/create_directory.h" // Não existe ainda (RED)
#include "application/icreator.h"       // Não existe ainda (RED)
#include "domain/directory.h"

#include <string>
#include <memory>

// PONTO 3 (TDD): Criando um Mock para a nossa interface.
// Ele simula a infraestrutura, permitindo testar a lógica da aplicação em isolamento.
class MockDirectoryCreator : public nautix::application::IDirectoryCreator {
public:
    // Armazena os dados da chamada para verificação posterior
    std::string called_with_path;

    // Implementação da interface que apenas registra a chamada
    std::expected<nautix::domain::Directory, std::error_code> create(
        const std::filesystem::path& path) override {

        called_with_path = path.string();

        // Retorna um objeto de domínio de sucesso para o teste
        return nautix::domain::Directory(path);
    }
};

TEST_CASE("CreateDirectory use case") {
    SECTION("should call the creator with the correct path") {
        // --- ARRANGE ---
        auto mock_creator = std::make_shared<MockDirectoryCreator>();

        // PONTO 4 (Clean Code): Injeção de Dependência. O caso de uso recebe
        // a dependência (o "criador") pelo construtor, em vez de criá-la internamente.
        // Usamos std::shared_ptr para gerenciar a vida do mock.
        nautix::application::CreateDirectory use_case(mock_creator);

        const std::filesystem::path parent_path = "/home/user";
        const std::string new_dir_name = "Music";

        // --- ACT ---
        auto result = use_case.execute(parent_path, new_dir_name);

        // --- ASSERT ---
        // Verificamos se a lógica do caso de uso funcionou como esperado
        REQUIRE(result.has_value());
        REQUIRE(mock_creator->called_with_path == "/home/user/Music");
    }
}