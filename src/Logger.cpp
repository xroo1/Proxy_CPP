#include "Logger.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

// Inicialização de variáveis estáticas
std::unordered_map<std::string, std::shared_ptr<Logger>> LogManager::loggers;
std::mutex LogManager::manager_mutex;
std::shared_ptr<Logger> LogManager::general_logger;
std::string LogManager::log_base_path = "./logs";
bool LogManager::debug_mode = false;

// Implementação Logger
Logger::Logger(const std::string& app, const std::string& base_path, bool debug) 
    : app_name(app), debug_mode(debug) {
    
    Utils::createDirectory(base_path);
    log_path = base_path + "/" + app + ".log";
    log_file.open(log_path, std::ios::app);
    
    if (!log_file.is_open()) {
        std::cerr << "ERRO: Não foi possível abrir " << log_path << std::endl;
    }
}

Logger::~Logger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

std::string Logger::getTimestamp() {
    return Utils::getTimestamp();
}

void Logger::write(const std::string& message, const std::string& level) {
    std::lock_guard<std::mutex> lock(log_mutex);
    
    std::string log_entry = "[" + getTimestamp() + "] [" + level + "] " + message;
    
    if (log_file.is_open()) {
        log_file << log_entry << std::endl;
        log_file.flush();
    }
    
    // CORREÇÃO: Use o método getter estático em vez de acessar a variável diretamente
    if (!LogManager::isDebugMode() || level == "ERROR" || level == "WARNING") {
        std::cout << "[" << app_name << "] " << log_entry << std::endl;
    }
}

void Logger::debug(const std::string& message) {
    if (debug_mode) {
        write(message, "DEBUG");
    }
}

void Logger::info(const std::string& message) {
    write(message, "INFO");
}

void Logger::warning(const std::string& message) {
    write(message, "WARNING");
}

void Logger::error(const std::string& message) {
    write(message, "ERROR");
}

std::string Logger::getLogPath() const {
    return log_path;
}

void Logger::setDebugMode(bool debug) {
    debug_mode = debug;
}

// Implementação LogManager
void LogManager::initialize(const std::string& base_path, bool debug) {
    log_base_path = base_path;
    debug_mode = debug;
    Utils::createDirectory(base_path);
}

std::shared_ptr<Logger> LogManager::getLogger(const std::string& app_name) {
    std::lock_guard<std::mutex> lock(manager_mutex);
    
    if (loggers.find(app_name) == loggers.end()) {
        loggers[app_name] = std::make_shared<Logger>(app_name, log_base_path, debug_mode);
        getGeneralLogger()->info("Novo logger criado para: " + app_name);
    }
    
    return loggers[app_name];
}

std::shared_ptr<Logger> LogManager::getGeneralLogger() {
    if (!general_logger) {
        general_logger = std::make_shared<Logger>("myproxy_general", log_base_path, debug_mode);
    }
    return general_logger;
}

void LogManager::listActiveLoggers() {
    std::lock_guard<std::mutex> lock(manager_mutex);
    auto general = getGeneralLogger();
    
    general->info("=== Loggers Ativos ===");
    for (const auto& pair : loggers) {
        general->info("App: " + pair.first + " -> " + pair.second->getLogPath());
    }
}

void LogManager::cleanup() {
    std::lock_guard<std::mutex> lock(manager_mutex);
    loggers.clear();
    general_logger.reset();
}
