cmake_minimum_required(VERSION 3.28)

project(nautix
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing globally
include(CTest)
enable_testing()

# Enable strict warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Dependencies: GTKMM
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

add_subdirectory(src)
add_subdirectory(tests)

# Main executable
add_executable(nautix
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:application>
    $<TARGET_OBJECTS:domain>
    $<TARGET_OBJECTS:infra>
    # $<TARGET_OBJECTS:ui> # Uncomment when UI is ready
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

#link_directories(${GTKMM_LIBRARY_DIRS})

