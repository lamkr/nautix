set(MODULE_NAME domain)

# Define a biblioteca que representará nosso módulo
add_library(${MODULE_NAME}
    directory.cpp
)

# Declara o arquivo de interface do módulo
target_sources(${MODULE_NAME} PUBLIC
    FILE_SET CXX_MODULES
    FILES
    domain.cppm
    directory.cppm
    local_time.cppm
    owner.cppm
)

# Expõe o nome lógico do módulo para outros alvos do CMake
target_compile_features(${MODULE_NAME} PUBLIC cxx_std_23)
set_target_properties(${MODULE_NAME} PROPERTIES CXX_EXPORTED_MODULE_NAME "nautix.domain")
