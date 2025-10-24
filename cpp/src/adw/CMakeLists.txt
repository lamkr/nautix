add_library(adw STATIC)

file(GLOB_RECURSE ADW_SOURCES
    CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/**/*.cpp"
)
file(GLOB_RECURSE ADW_HEADERS
    CONFIGURE_DEPENDS
        "include/*.h"
        "include/**/*.h"
)

message(STATUS "ADW_SOURCES: ${ADW_SOURCES}")
message(STATUS "ADW_HEADERS: ${ADW_HEADERS}")

target_include_directories(adw
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${GTKMM_INCLUDE_DIRS}
        ${LIBADWAITA_INCLUDE_DIRS}
)

target_link_libraries(adw
    PUBLIC
        ${GTKMM_LIBRARIES}
        ${LIBADWAITA_LIBRARIES}
)

target_compile_definitions(adw
    PRIVATE
    ${GTKMM_DEFINITIONS}
    ${LIBADWAITA_DEFINITIONS}
)

target_sources(adw
    PRIVATE ${ADW_SOURCES}
    PUBLIC FILE_SET HEADERS FILES ${ADW_HEADERS}
)
