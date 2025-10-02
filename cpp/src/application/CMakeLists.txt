set(MODULE_NAME application)

add_library(${MODULE_NAME}
    list_directories.cpp
)

target_sources(${MODULE_NAME} PUBLIC
    FILE_SET CXX_MODULES
    FILES
    application.cppm
    list_directories.cppm
)

target_link_libraries(application PUBLIC domain)
target_compile_features(${MODULE_NAME} PUBLIC cxx_std_23)
set_target_properties(${MODULE_NAME} PROPERTIES CXX_EXPORTED_MODULE_NAME "nautix.application")
