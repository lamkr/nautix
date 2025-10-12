add_library(infra STATIC)

# --- Fontes ---
file(GLOB_RECURSE INFRA_SOURCES
        CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/**/*.cpp"
)
file(GLOB_RECURSE INFRA_HEADERS
        CONFIGURE_DEPENDS
        "include/*.h"
        "include/**/*.h"
)

message(STATUS "INFRA_SOURCES: ${INFRA_SOURCES}")
message(STATUS "INFRA_HEADERS: ${INFRA_HEADERS}")

target_sources(infra
    PRIVATE ${INFRA_SOURCES}
    PUBLIC FILE_SET HEADERS FILES ${INFRA_HEADERS}
)

# --- Diretórios de Include ---
target_include_directories(infra
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# --- Dependências ---
# A camada de infraestrutura DEPENDE da camada de aplicação.
target_link_libraries(infra
    PUBLIC
        common
        application
)