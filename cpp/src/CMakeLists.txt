# Biblioteca principal do projeto
add_library(nautix_core
    domain/file.cpp
    domain/directory.cpp
    application/list_directories.cpp
    # outras implementações
)

target_include_directories(nautix_core
    PUBLIC ${PROJECT_SOURCE_DIR}/include)
