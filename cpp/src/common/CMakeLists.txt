# Define a biblioteca 'common' como uma biblioteca de INTERFACE.
# Isso significa que ela não produz um arquivo .a ou .so, mas serve para
# agrupar propriedades para outros alvos.
add_library(common STATIC)

# Encontra os fontes
file(GLOB_RECURSE COMMON_SOURCES
        CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/**/*.cpp"
)

target_sources(common PRIVATE ${COMMON_SOURCES})

# Adiciona o diretório de include à interface da biblioteca.
# Qualquer alvo que se vincular a 'common' automaticamente terá este
# diretório em seu caminho de busca de includes.
target_include_directories(common
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Como i18n.h depende da biblioteca Gettext (libintl), nós vinculamos
# a biblioteca 'common' com Gettext.
# Isso garante que qualquer alvo que usar 'common' também será vinculado
# automaticamente com Gettext.
target_link_libraries(common
    INTERFACE
        ${GETTEXT_LIBRARIES}
        spdlog::spdlog
)

target_include_directories(common
    INTERFACE
        ${GETTEXT_INCLUDE_DIRS}
)