cmake_minimum_required(VERSION 3.28)

set(TEST_SOURCES
        domain/test_directory.cpp
)

add_executable(test_list_directories ${TEST_SOURCES})

target_link_libraries(test_list_directories
        PRIVATE
        Catch2::Catch2WithMain
# O domínio é uma dependência transitiva, mas é bom ser explícito
        nautix.domain
)

#include(FetchContent)
#FetchContent_Declare(
#    catch2
#    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#    GIT_TAG v3.10.0
#)
#FetchContent_MakeAvailable(catch2)
#
## Register tests automatically with CTest
#include(Catch)
#catch_discover_tests(test_list_directories)
#
## Enable ASAN in Debug builds
#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    target_compile_options(test_list_directories PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
#    target_link_options(test_list_directories PRIVATE -fsanitize=address,undefined)
#endif()

# Adiciona o executável à suíte de testes do CTest
add_test(NAME unit_tests COMMAND run_tests)
