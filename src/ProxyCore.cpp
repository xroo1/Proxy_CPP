#include "ProxyCore.h"
#include "utils.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>

ProxyCore::ProxyCore(const ProxyConfig& cfg, std::shared_ptr<Logger> log)
    : config(cfg), chain_manager(cfg, log), logger(log), running(false), listen_port(1080) {
}

ProxyCore::~ProxyCore() {
    stop();
}

bool ProxyCore::initialize() {
    logger->info("Inicializando MyProxy Core");
    logger->info("Chain type: " + config.chain_type);
    logger->info("Proxies disponíveis: " + std::to_string(config.proxy_list.size()));
    
    return true;
}

void ProxyCore::start(int port) {
    listen_port = port;
    running = true;
    
    // Aqui iria a implementação do servidor proxy
    // Similar à versão anterior, mas usando chain_manager
    
    logger->info("Proxy core iniciado na porta " + std::to_string(port));
}

void ProxyCore::stop() {
    running = false;
    logger->info("Proxy core parado");
}

std::string ProxyCore::detectAppName(const std::string& client_ip, const std::string& cmd_line) {
    // Tenta extrair nome do app da linha de comando
    if (!cmd_line.empty()) {
        auto parts = Utils::split(cmd_line, ' ');
        if (!parts.empty()) {
            // Pega o nome do executável
            std::string exe = parts[0];
            size_t last_slash = exe.find_last_of('/');
            if (last_slash != std::string::npos) {
                exe = exe.substr(last_slash + 1);
            }
            return exe;
        }
    }
    
    // Fallback para IP-based
    return "app_" + client_ip;
}

// No método executeThroughProxy do ProxyCore.cpp
int ProxyCore::executeThroughProxy(const std::string& command, const std::vector<std::string>& args) {
    logger->info("Executando comando através do proxy: " + command);
    
    // Seleciona a chain de proxies
    auto chain = chain_manager.selectChain();
    if (chain.empty()) {
        logger->error("Nenhum proxy disponível na chain");
        return 1;
    }
    
    // Pega o primeiro proxy da chain (para simplificar)
    auto& proxy = chain[0];
    logger->info("Usando proxy: " + proxy.type + "://" + proxy.host + ":" + std::to_string(proxy.port));
    
    // Configura environment variables para o proxy
    std::string proxy_url;
    if (proxy.type == "http" || proxy.type == "https") {
        proxy_url = "http://" + proxy.host + ":" + std::to_string(proxy.port);
        setenv("HTTP_PROXY", proxy_url.c_str(), 1);
        setenv("HTTPS_PROXY", proxy_url.c_str(), 1);
        setenv("http_proxy", proxy_url.c_str(), 1);
        setenv("https_proxy", proxy_url.c_str(), 1);
    } else if (proxy.type == "socks4" || proxy.type == "socks5") {
        proxy_url = "socks5://" + proxy.host + ":" + std::to_string(proxy.port);
        setenv("ALL_PROXY", proxy_url.c_str(), 1);
        setenv("all_proxy", proxy_url.c_str(), 1);
        setenv("SOCKS_PROXY", proxy_url.c_str(), 1);
        setenv("socks_proxy", proxy_url.c_str(), 1);
    }
    
    logger->info("Proxy configurado: " + proxy_url);
    
    // Monta o comando completo
    std::string full_cmd = command;
    for (const auto& arg : args) {
        full_cmd += " " + arg;
    }
    
    logger->info("Executando: " + full_cmd);
    
    // Executa o comando
    int result = system(full_cmd.c_str());
    
    // Limpa environment variables
    unsetenv("HTTP_PROXY");
    unsetenv("HTTPS_PROXY");
    unsetenv("http_proxy");
    unsetenv("https_proxy");
    unsetenv("ALL_PROXY");
    unsetenv("all_proxy");
    unsetenv("SOCKS_PROXY");
    unsetenv("socks_proxy");
    
    return result;
}
