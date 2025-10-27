#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>

// Estrutura para um proxy individual
struct ProxyServer {
    std::string type;     // socks4, socks5, http
    std::string host;
    int port;
    std::string username;
    std::string password;
    
    ProxyServer() : port(0) {}
};

// Estrutura principal de configuração
struct ProxyConfig {
    // Chain settings
    std::string chain_type;           // dynamic_chain, strict_chain, random_chain, round_robin_chain
    int chain_len;                    // Comprimento da chain
    bool quiet_mode;                  // Modo silencioso
    bool proxy_dns;                   // Proxy DNS requests
    bool proxy_dns_old;               // Método DNS antigo
    int remote_dns_subnet;            // Subnet para DNS remoto
    int tcp_read_time_out;            // Timeout leitura TCP (ms)
    int tcp_connect_time_out;         // Timeout conexão TCP (ms)
    
    // Lista de proxies
    std::vector<ProxyServer> proxy_list;
    
    // Redes locais (exclusões)
    std::vector<std::string> localnet;
    
    // Configurações DNAT
    std::vector<std::string> dnat_rules;
    
    ProxyConfig();
};

class ConfigParser {
private:
    std::string config_path;
    
    // Parse individual lines
    bool parseLine(const std::string& line, ProxyConfig& config);
    ProxyServer parseProxyLine(const std::string& line);

public:
    ConfigParser(const std::string& path = "config/myproxy.conf");
    
    bool loadConfig(ProxyConfig& config);
    bool saveConfig(const ProxyConfig& config);
    
    void setConfigPath(const std::string& path);
    std::string getConfigPath() const;
};

#endif
