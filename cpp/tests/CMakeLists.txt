enable_testing()

set(TEST_SOURCES
        application/ListItems.test.cpp
        application/DeleteItems.test.cpp
)

add_executable(tests
        ${TEST_SOURCES})

target_link_libraries(tests
        PRIVATE
            application
            infra
            Catch2::Catch2WithMain
            trompeloeil
            ${GETTEXT_LIBRARIES}
)

# Adicione também os diretórios de include do Gettext
target_include_directories(tests
    PRIVATE
        ${GETTEXT_INCLUDE_DIRS}
)

include(CTest)
include(Catch)
catch_discover_tests(tests)

# Enable ASAN in Debug builds
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(tests PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(tests PRIVATE -fsanitize=address,undefined)
endif()
