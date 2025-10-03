set(MODULE_NAME infra)

add_library(${MODULE_NAME}
)

target_sources(${MODULE_NAME} PUBLIC
        FILE_SET CXX_MODULES
        FILES
        infra.cppm
)

target_compile_features(${MODULE_NAME} PUBLIC cxx_std_23)
set_target_properties(${MODULE_NAME} PROPERTIES CXX_EXPORTED_MODULE_NAME "nautix.infra")