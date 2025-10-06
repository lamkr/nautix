add_library(ui STATIC)

# --- Fontes ---
file(GLOB_RECURSE UI_SOURCES
        CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/**/*.cpp"
)
file(GLOB_RECURSE UI_HEADERS
        CONFIGURE_DEPENDS
        "include/*.h"
        "include/**/*.h"
)

message(STATUS "UI_SOURCES: ${UI_SOURCES}")
message(STATUS "UI_HEADERS: ${UI_HEADERS}")

target_sources(ui
    PRIVATE ${UI_SOURCES}
    PUBLIC FILE_SET HEADERS FILES ${UI_HEADERS}
)

# --- Diretórios de Include ---
target_include_directories(ui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# --- Dependências ---
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(GTKMM gtkmm-4.0 REQUIRED)

# A camada de UI DEPENDE da camada de infraestrutura.
target_link_libraries(ui
    PUBLIC
        common
        infra
        ${GTKMM_LIBRARIES}
)
