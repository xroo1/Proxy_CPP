#include <iostream>
#include <vector>
#include <string>
#include "ConfigParser.h"
#include "ProxyCore.h"
#include "Logger.h"

void showHelp() {
    std::cout << "MyProxy - Sistema de Proxy com Chains\n\n";
    std::cout << "Uso:\n";
    std::cout << "  ./myproxy <comando> [argumentos...]\n";
    std::cout << "  ./myproxy --help\n";
    std::cout << "  ./myproxy --version\n";
    std::cout << "  ./myproxy --config <arquivo.conf>\n\n";
    std::cout << "Exemplos:\n";
    std::cout << "  ./myproxy weechat\n";
    std::cout << "  ./myproxy firefox\n";
    std::cout << "  ./myproxy curl https://example.com\n";
    std::cout << "  ./myproxy --config /path/to/my.conf wget http://example.com\n\n";
    std::cout << "Os logs são salvos em /var/log/myproxy/ ou ./logs/\n";
}

void showVersion() {
    std::cout << "MyProxy 1.0.0\n";
    std::cout << "Sistema de Proxy com Chains e Logs Separados\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showHelp();
        return 1;
    }
    
    std::string command = argv[1];
    
    // Opções especiais
    if (command == "--help" || command == "-h") {
        showHelp();
        return 0;
    }
    
    if (command == "--version" || command == "-v") {
        showVersion();
        return 0;
    }
    
    // Configuração
    std::string config_path = "config/myproxy.conf";
    std::vector<std::string> app_args;
    
    // Parse argumentos
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--config" && i + 1 < argc) {
            config_path = argv[++i];
        } else if (arg[0] != '-') {
            // Primeiro argumento não-option é o comando
            if (app_args.empty()) {
                app_args.push_back(arg);
            } else {
                app_args.push_back(arg);
            }
        }
    }
    
    if (app_args.empty()) {
        std::cerr << "Erro: Nenhum comando especificado\n";
        showHelp();
        return 1;
    }
    
    // Carrega configuração
    ConfigParser parser(config_path);
    ProxyConfig config;
    
    if (!parser.loadConfig(config)) {
        std::cerr << "Erro: Não foi possível carregar configuração\n";
        return 1;
    }
    
    // Inicializa sistema de logs
    LogManager::initialize("/var/log/myproxy", !config.quiet_mode);
    auto logger = LogManager::getGeneralLogger();
    
    logger->info("=== MyProxy Iniciado ===");
    logger->info("Configuração: " + config_path);
    logger->info("Comando: " + app_args[0]);
    
    // Cria core do proxy
    ProxyCore proxy_core(config, logger);
    
    if (!proxy_core.initialize()) {
        logger->error("Falha na inicialização do proxy core");
        return 1;
    }
    
    // Executa comando
    std::string app_command = app_args[0];
    app_args.erase(app_args.begin()); // Remove o comando da lista de args
    
    int result = proxy_core.executeThroughProxy(app_command, app_args);
    
    logger->info("Comando finalizado com código: " + std::to_string(result));
    LogManager::cleanup();
    
    return result;
}
