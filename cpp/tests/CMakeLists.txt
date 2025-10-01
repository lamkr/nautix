cmake_minimum_required(VERSION 3.28)

include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.10.0
)
FetchContent_MakeAvailable(catch2)

# Teste do ListDirectories
add_executable(test_list_directories
    application/test_list_directories.cpp
        domain/test_directory.cpp
)

target_compile_options(test_list_directories PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
target_link_options(test_list_directories PRIVATE -fsanitize=address,undefined)

target_include_directories(test_list_directories
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${catch2_SOURCE_DIR}/single_include
)

target_link_libraries(test_list_directories
    PRIVATE nautix_core
    Catch2::Catch2WithMain
)

# Registrar teste para CTest (opcional)
include(CTest)
add_test(NAME ListDirectoriesTest COMMAND test_list_directories)


# Enable ASAN in Debug builds
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(test_list_directories PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(test_list_directories PRIVATE -fsanitize=address,undefined)
endif()