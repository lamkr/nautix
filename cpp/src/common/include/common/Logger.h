#pragma once

#include <spdlog/spdlog.h>
#include <memory>

// Wrapper simples para inicializar e acessar o logger global do Nautix
class Logger {
public:
    // Deve ser chamada uma única vez no início da aplicação (em main.cpp)
    static void init();

    // Acessa o logger singleton
    static std::shared_ptr<spdlog::logger>& get();
};