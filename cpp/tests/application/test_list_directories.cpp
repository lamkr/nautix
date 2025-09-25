#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/directory.h"
#include "application/list_directories.h"

#include <filesystem>
#include <fstream>

// Helper para criar um ambiente temporário de teste
struct TempDir {
    std::filesystem::path path;

    TempDir() {
        path = std::filesystem::temp_directory_path() /
               std::filesystem::path("nautix_test_" + std::to_string(::getpid()));
        std::filesystem::create_directory(path);
    }

    ~TempDir() {
        std::filesystem::remove_all(path);
    }
};

TEST_CASE("Listar subdiretórios a partir de um diretório raiz") {
    using namespace nautix::domain;
    using namespace nautix::application;

    TempDir tmp;

    // Criar subdiretórios
    auto sub1 = tmp.path / "subdir1";
    auto sub2 = tmp.path / "subdir2";
    std::filesystem::create_directory(sub1);
    std::filesystem::create_directory(sub2);

    // Criar um arquivo (não deve aparecer na listagem de diretórios)
    std::ofstream(tmp.path / "arquivo.txt").put('a');

    Directory root(tmp.path.string());
    ListDirectories useCase;

    auto result = useCase.execute(root);

    REQUIRE(result.size() == 2);
    REQUIRE((result[0].path() == sub1.string() || result[0].path() == sub2.string()));
    REQUIRE((result[1].path() == sub1.string() || result[1].path() == sub2.string()));
}

TEST_CASE("Diretório sem subdiretórios deve retornar lista vazia") {
    using namespace nautix::domain;
    using namespace nautix::application;

    TempDir tmp;

    Directory root(tmp.path.string());
    ListDirectories useCase;

    auto result = useCase.execute(root);

    REQUIRE(result.empty());
}
