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
# 2. FIND SYSTEM & EXTERNAL DEPENDENCIES (CENTRALIZED)
# All dependencies are found or fetched here, once.
# ==============================================================================
include(FetchContent)

find_package(PkgConfig REQUIRED)

# --- System Libraries (via pkg-config) ---
find_package(Gettext REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
pkg_check_modules(SYSTEMD REQUIRED libsystemd)
pkg_check_modules(LIBADWAITA REQUIRED libadwaita-1)

FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.11.0)
FetchContent_MakeAvailable(Catch2)

# ATTENTION: must goes before SPDLOG
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 11.2.0)
FetchContent_MakeAvailable(fmt)

# ATTENTION: Before, must be installed: sudo apt-get install libsystemd-dev
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.15.3)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt")
FetchContent_MakeAvailable(spdlog)

# ATTENTION: Before, must be installed: sudo apt-get install gettext
FetchContent_Declare(trompeloeil GIT_REPOSITORY https://github.com/rollbear/trompeloeil.git GIT_TAG v47)
FetchContent_MakeAvailable(trompeloeil)

# ==============================================================================
# 3. CONFIGURE SUBDIRECTORIES & MAIN TARGET
# ==============================================================================
add_subdirectory(src)
add_subdirectory(tests)

# Main executable
add_executable(nautix
	src/main.cpp
)

target_link_libraries(nautix
    PRIVATE
    	ui
        ${GTKMM_LIBRARIES}
        ${LIBADWAITA_LIBRARIES}
        ${Gettext_LIBRARIES}
)

target_include_directories(nautix
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ui/include
        ${LIBADWAITA_INCLUDE_DIRS}
        ${GTKMM_INCLUDE_DIRS}
        ${Gettext_INCLUDE_DIRS}
)
#
#link_directories(
#    ${GTKMM_LIBRARY_DIRS}
#)
#
