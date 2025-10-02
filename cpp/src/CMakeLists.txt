
#add_subdirectory(core)
#add_subdirectory(application)
add_subdirectory(domain)
#add_subdirectory(infra)
#add_subdirectory(ui)

# Core library that groups all components
add_library(nautix_core
    $<TARGET_OBJECTS:domain>
)

# Expose headers to any target linking nautix_core (tests, executables, etc.)
target_include_directories(nautix_core
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)


