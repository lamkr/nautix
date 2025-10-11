#include "common/Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/systemd_sink.h>
#include <vector>

// Variável estática para guardar nosso logger
    static std::shared_ptr<spdlog::logger> s_logger;

void Logger::init() {
    // Lista de "sinks" (destinos) para o nosso log.
    std::vector<spdlog::sink_ptr> sinks;

    // 1. Sink para o console, com cores, para desenvolvimento
    sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

    // 2. Sink para o systemd/journald, para integração com o sistema (GNOME Logs)
    // Isso só funciona em Linux, o nosso alvo.
    try {
        sinks.emplace_back(std::make_shared<spdlog::sinks::systemd_sink_mt>());
    } catch (const spdlog::spdlog_ex& ex) {
        // Se o systemd não estiver disponível, apenas avise no console
        spdlog::warn("Systemd sink failed to initialize: {}", ex.what());
    }


    // Cria o logger que envia mensagens para todos os sinks configurados
    s_logger = std::make_shared<spdlog::logger>("nautix", begin(sinks), end(sinks));
    
    // Registra o logger para acessá-lo globalmente se necessário
    spdlog::register_or_replace(s_logger);
    
    // Define o nível de log padrão. 'trace' é o mais detalhado.
    s_logger->set_level(spdlog::level::trace);
    
    // Define o padrão de formatação das mensagens
    s_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");

    s_logger->info("Logger initialized.");
}

std::shared_ptr<spdlog::logger>& Logger::get() {
    if (!s_logger) {
        init();
    }
    return s_logger;
}