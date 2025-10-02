#add_subdirectory(core)
#add_subdirectory(application)
add_subdirectory(domain)
#add_subdirectory(infra)
#add_subdirectory(ui)

# Core library that groups all components
add_library(nautix_core
    $<TARGET_OBJECTS:domain>
)
