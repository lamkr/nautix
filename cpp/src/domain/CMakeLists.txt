# Define a biblioteca que representará nosso módulo
add_library(domain
        directory.cpp
)

# PONTO 5: Declara o arquivo de interface do módulo
target_sources(domain PUBLIC
        FILE_SET CXX_MODULES
        FILES
        domain.cppm
        directory.ixx
)

# Expõe o nome lógico do módulo para outros alvos do CMake
target_compile_features(domain PUBLIC cxx_std_23)
set_target_properties(domain PROPERTIES CXX_EXPORTED_MODULE_NAME "nautix.domain")
