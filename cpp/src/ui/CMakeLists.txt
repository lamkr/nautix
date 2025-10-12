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

# --- Diretórios de Include ---
target_include_directories(ui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${GTKMM_INCLUDE_DIRS}
        ${LIBADWAITA_INCLUDE_DIRS}
)

# A camada de UI DEPENDE da camada de infraestrutura.
target_link_libraries(ui
    PUBLIC
        common
        infra
        ${GTKMM_LIBRARIES}
        ${LIBADWAITA_LIBRARIES}
)

target_compile_definitions(ui
    PRIVATE
    ${GTKMM_DEFINITIONS}
    ${LIBADWAITA_DEFINITIONS}
)

# Add the generated C file to the UI library
target_sources(ui
    PRIVATE
        ${UI_SOURCES}
    PUBLIC FILE_SET HEADERS FILES ${UI_HEADERS}
)




