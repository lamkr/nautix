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
)

target_include_directories(test_list_directories
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${catch2_SOURCE_DIR}/single_include
)

target_link_libraries(test_list_directories
    PRIVATE nautix_core Catch2::Catch2WithMain)

# Registrar teste para CTest (opcional)
include(CTest)
add_test(NAME ListDirectoriesTest COMMAND test_list_directories)
