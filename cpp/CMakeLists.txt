cmake_minimum_required(VERSION 3.28)

message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}") # Provides a unique ID like "GNU", "Clang", "MSVC"

project(nautix
    VERSION 1.0
    LANGUAGES CXX
)

# ==============================================================================
# 1. SETUP PROJECT-WIDE SETTINGS
# ==============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable strict warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ==============================================================================
# 2. INTEGRATE TROMPE-L'OEIL (MOCKING LIBRARY)
# FetchContent is the modern way to manage dependencies like this.
# ==============================================================================
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# ATTENTION: Before, must be installed gettext at terminal.
FetchContent_Declare(
    trompeloeil
    GIT_REPOSITORY https://github.com/rollbear/trompeloeil.git
    GIT_TAG        v47
)
FetchContent_MakeAvailable(trompeloeil)

# Dependencies
find_package(PkgConfig REQUIRED)
find_package(Gettext REQUIRED) # i18n
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

add_subdirectory(src)
add_subdirectory(tests)

# Main executable
add_executable(nautix
    src/main.cpp
)

target_link_libraries(nautix
    PRIVATE
        ${GTKMM_LIBRARIES}
        ${Gettext_LIBRARIES}
)

target_include_directories(nautix
    PRIVATE
        ${GTKMM_INCLUDE_DIRS}
        ${Gettext_INCLUDE_DIRS}
)

link_directories(${GTKMM_LIBRARY_DIRS})

