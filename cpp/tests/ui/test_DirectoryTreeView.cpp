#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/directory.h"
#include "application/list_directories.h"

#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

#include "core/fs.h"
#include "infra/system_metadata_provider.h"

namespace fs = std::filesystem;
namespace domain = nautix::domain;
namespace app = nautix::application;
namespace infra = nautix::infra;

// 1. Testa inicialização com diretórios fixos
TEST_CASE("DirectoryTreeView initializes with default directories", "[ui][init]") {
    nautix::ui::DirectoryTreeView tree;

    auto dirs = tree.getDirectories();

    REQUIRE(std::find(dirs.begin(), dirs.end(), "Home") != dirs.end());
    REQUIRE(std::find(dirs.begin(), dirs.end(), "Documents") != dirs.end());
    REQUIRE(std::find(dirs.begin(), dirs.end(), "Downloads") != dirs.end());
    REQUIRE(std::find(dirs.begin(), dirs.end(), "Music") != dirs.end());
    REQUIRE(std::find(dirs.begin(), dirs.end(), "Pictures") != dirs.end());
    REQUIRE(std::find(dirs.begin(), dirs.end(), "Videos") != dirs.end());
    REQUIRE(std::find(dirs.begin(), dirs.end(), "/") != dirs.end());
}

// 2. Testa favoritos
TEST_CASE("DirectoryTreeView can add and remove favorites", "[ui][favorites]") {
    nautix::ui::DirectoryTreeView tree;

    tree.addFavorite("Music");
    REQUIRE(tree.isFavorite("Music"));

    tree.removeFavorite("Music");
    REQUIRE_FALSE(tree.isFavorite("Music"));
}

// 3. Testa navegação
TEST_CASE("DirectoryTreeView navigates correctly", "[ui][navigation]") {
    nautix::ui::DirectoryTreeView tree;

    REQUIRE_NOTHROW(tree.navigateTo("Documents"));
    REQUIRE(tree.getCurrentDirectory() == "Documents");

    REQUIRE_THROWS(tree.navigateTo("NonExistent"));
}

// 4. Testa integração com i18n
TEST_CASE("DirectoryTreeView supports i18n labels", "[ui][i18n]") {
    nautix::ui::DirectoryTreeView tree;

    // Exemplo fictício: deve traduzir "Documents" para "Documentos" (pt_BR)
    tree.setLocale("pt_BR");
    REQUIRE(tree.getLocalizedLabel("Documents") == "Documentos");
}
