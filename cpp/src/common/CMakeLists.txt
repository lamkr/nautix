# A biblioteca 'common' é STATIC porque agora possui arquivos .cpp (Logger.cpp)
add_library(common STATIC)

file(GLOB_RECURSE COMMON_SOURCES
        CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/**/*.cpp"
)
file(GLOB_RECURSE COMMON_HEADERS
        CONFIGURE_DEPENDS
        "include/*.h"
        "include/**/*.h"
)

message(STATUS "COMMON_SOURCES: ${COMMON_SOURCES}")
message(STATUS "COMMON_HEADERS: ${COMMON_HEADERS}")

target_sources(common
    PRIVATE ${COMMON_SOURCES}
    PUBLIC FILE_SET HEADERS FILES ${COMMON_HEADERS}
)

# Adiciona os diretórios de include.
# Usamos PUBLIC para que tanto o 'common' quanto seus consumidores
# possam encontrar todos os headers necessários.
target_include_directories(common
    PUBLIC
        # 1. O include da própria biblioteca 'common'
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        
        # 2. O include da dependência 'spdlog' (ESSA É A LINHA CRÍTICA!)
        $<BUILD_INTERFACE:${spdlog_SOURCE_DIR}/include> # <-- VERIFIQUE SE ESTA LINHA ESTÁ PRESENTE
        
        # 3. O include da dependência 'Gettext'
        ${GETTEXT_INCLUDE_DIRS}
)

# Vincula as dependências.
# Usamos PUBLIC para que os consumidores também sejam vinculados.
target_link_libraries(common
    PUBLIC
        spdlog::spdlog
        ${GETTEXT_LIBRARIES}
        ${SYSTEMD_LIBRARIES}
)

