add_library(application STATIC)

# --- Fontes ---
file(GLOB_RECURSE APP_SOURCES
        CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/**/*.cpp"
)
file(GLOB_RECURSE APP_HEADERS "include/**/*.h")

message(STATUS "APP_SOURCES: ${APP_SOURCES}")
message(STATUS "APP_HEADERS: ${APP_HEADERS}")

target_sources(application
    PRIVATE ${APP_SOURCES}
    PUBLIC FILE_SET HEADERS FILES ${APP_HEADERS}
)

# --- Diretórios de Include ---
target_include_directories(application
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# --- Dependências ---
# A camada de aplicação DEPENDE da camada de domínio.
# PUBLIC garante que quem depender de 'application' também enxergue 'domain'.
target_link_libraries(application PUBLIC domain)