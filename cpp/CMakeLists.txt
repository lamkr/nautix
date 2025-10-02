cmake_minimum_required(VERSION 4.0.2)

#set(CMAKE_CXX_COMPILER clang++-20)
#set(CMAKE_C_COMPILER clang-20)

message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}") # Provides a unique ID like "GNU", "Clang", "MSVC"

project(nautix
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Enable strict warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

## Adicionar o FetchContent para baixar o Catch2 automaticamente
#include(FetchContent)
#FetchContent_Declare(
#        Catch2
#        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#        GIT_TAG        v3.4.0
#)
#FetchContent_MakeAvailable(Catch2)

# Enable testing globally
include(CTest)
enable_testing()

# Dependencies: GTKMM
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

add_subdirectory(src)
add_subdirectory(tests)

# Main executable
add_executable(nautix
    #$<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:application>
    $<TARGET_OBJECTS:domain>
    #$<TARGET_OBJECTS:infra>
    # $<TARGET_OBJECTS:ui>
    src/main.cpp
)

target_link_libraries(nautix
    PRIVATE
        nautix_core
        ${GTKMM_LIBRARIES}
)

target_include_directories(nautix
    PRIVATE
        ${GTKMM_INCLUDE_DIRS}
)

link_directories(${GTKMM_LIBRARY_DIRS})

