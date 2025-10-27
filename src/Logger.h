#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include <memory>

class Logger {
private:
    std::ofstream log_file;
    std::mutex log_mutex;
    std::string app_name;
    std::string log_path;
    bool debug_mode;

    std::string getTimestamp();

public:
    Logger(const std::string& app, const std::string& base_path = "./logs", bool debug = false);
    ~Logger();

    void write(const std::string& message, const std::string& level = "INFO");
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);

    std::string getLogPath() const;
    void setDebugMode(bool debug);
    bool getDebugMode() const { return debug_mode; } // Adicionei este getter
};

class LogManager {
private:
    static std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;
    static std::mutex manager_mutex;
    static std::shared_ptr<Logger> general_logger;
    static std::string log_base_path;
    static bool debug_mode;

public:
    static void initialize(const std::string& base_path = "./logs", bool debug = false);
    static std::shared_ptr<Logger> getLogger(const std::string& app_name);
    static std::shared_ptr<Logger> getGeneralLogger();
    static void listActiveLoggers();
    static void cleanup();
    static bool isDebugMode() { return debug_mode; } // Adicionei este getter estático
};

#endif
