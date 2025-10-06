add_library(domain STATIC)

file(GLOB_RECURSE DOMAIN_SOURCES
        CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/**/*.cpp"
)
file(GLOB_RECURSE DOMAIN_HEADERS
        CONFIGURE_DEPENDS
        "include/*.h"
        "include/**/*.h"
)

message(STATUS "DOMAIN_SOURCES: ${DOMAIN_SOURCES}")
message(STATUS "DOMAIN_HEADERS: ${DOMAIN_HEADERS}")

target_sources(domain
    PRIVATE ${DOMAIN_SOURCES}
    PUBLIC FILE_SET HEADERS FILES ${DOMAIN_HEADERS}
)

target_include_directories(domain
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
