enable_testing()

set(TEST_SOURCES
    application/ListDirectories.integration-test.cpp
    application/RenameItem.integration-test.cpp
    application/CopyItem.integration-test.cpp
    application/MoveItems.integration-test.cpp
    application/DeleteItems.test.cpp
    application/DeleteItems.integration-test.cpp
    application/ListDirectories.test.cpp
    ui/main_window.test.cpp
)

add_executable(tests
    ${TEST_SOURCES}
)

# Adicione também os diretórios de include do Gettext
target_include_directories(tests
    PRIVATE
        ${Gettext_INCLUDE_DIRS}
        ${GTKMM_INCLUDE_DIRS}
        ${LIBADWAITA_INCLUDE_DIRS}
)

target_link_libraries(tests
    PRIVATE
        ui
        Catch2::Catch2WithMain
        trompeloeil
        ${Gettext_INCLUDE_DIRS}
        ${GTKMM_LIBRARIES}
        ${LIBADWAITA_LIBRARIES}
)

include(CTest)
include(Catch)
catch_discover_tests(tests)

# Enable ASAN in Debug builds
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(tests PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(tests PRIVATE -fsanitize=address,undefined)
endif()
