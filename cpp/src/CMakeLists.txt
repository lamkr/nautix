add_subdirectory(core)
add_subdirectory(application)
add_subdirectory(domain)
add_subdirectory(infra)
add_subdirectory(ui)

add_executable(nautix
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:application>
    $<TARGET_OBJECTS:domain>
    $<TARGET_OBJECTS:infra>
    #$<TARGET_OBJECTS:ui>
    main.cpp
        infra/system_metadata_provider.cpp
)

# Biblioteca principal do projeto
add_library(nautix_core
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:application>
    $<TARGET_OBJECTS:domain>
    $<TARGET_OBJECTS:infra>
)