cmake_minimum_required(VERSION 3.28)

project(nautix
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ativa warnings importantes
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Busca dependências do GTKmm
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

include_directories(
    ${GTKMM_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/application
    ${PROJECT_SOURCE_DIR}/src/infra
    ${PROJECT_SOURCE_DIR}/src/domain
    ${PROJECT_SOURCE_DIR}/src/ui
)
add_subdirectory(src)
add_subdirectory(tests)

link_directories(${GTKMM_LIBRARY_DIRS})

target_link_libraries(nautix
    ${GTKMM_LIBRARIES}
)
