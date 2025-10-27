#include "ConfigParser.h"
#include "utils.h"
#include <fstream>
#include <iostream>

ProxyConfig::ProxyConfig() :
    chain_type("dynamic_chain"),
    chain_len(2),
    quiet_mode(false),
    proxy_dns(true),
    proxy_dns_old(false),
    remote_dns_subnet(224),
    tcp_read_time_out(15000),
    tcp_connect_time_out(8000) {
}

ConfigParser::ConfigParser(const std::string& path) : config_path(path) {
}

bool ConfigParser::parseLine(const std::string& line, ProxyConfig& config) {
    std::string trimmed = Utils::trim(line);
    
    // Ignora comentários e linhas vazias
    if (trimmed.empty() || trimmed[0] == '#') {
        return true;
    }
    
    // Verifica se é seção [ProxyList]
    if (Utils::trim(Utils::toLower(trimmed)) == "[proxylist]") {
        // A lista de proxies é tratada no loop principal
        return true;
    }
    
    // Parse key = value
    size_t pos = trimmed.find('=');
    if (pos == std::string::npos) {
        return true; // Ignora linhas sem '='
    }
    
    std::string key = Utils::trim(Utils::toLower(trimmed.substr(0, pos)));
    std::string value = Utils::trim(trimmed.substr(pos + 1));
    
    // Parse das configurações
    if (key == "chain_type") {
        config.chain_type = value;
    }
    else if (key == "chain_len") {
        config.chain_len = std::stoi(value);
    }
    else if (key == "quiet_mode") {
        config.quiet_mode = (Utils::toLower(value) == "on");
    }
    else if (key == "proxy_dns") {
        config.proxy_dns = (Utils::toLower(value) == "on");
    }
    else if (key == "proxy_dns_old") {
        config.proxy_dns_old = (Utils::toLower(value) == "on");
    }
    else if (key == "remote_dns_subnet") {
        config.remote_dns_subnet = std::stoi(value);
    }
    else if (key == "tcp_read_time_out") {
        config.tcp_read_time_out = std::stoi(value);
    }
    else if (key == "tcp_connect_time_out") {
        config.tcp_connect_time_out = std::stoi(value);
    }
    else if (key == "localnet") {
        config.localnet.push_back(value);
    }
    else if (key == "dnat") {
        config.dnat_rules.push_back(value);
    }
    
    return true;
}

ProxyServer ConfigParser::parseProxyLine(const std::string& line) {
    ProxyServer proxy;
    std::string trimmed = Utils::trim(line);
    
    if (trimmed.empty() || trimmed[0] == '#') {
        return proxy; // Retorna proxy vazio
    }
    
    auto parts = Utils::split(trimmed, ' ');
    if (parts.size() < 3) {
        return proxy;
    }
    
    proxy.type = Utils::toLower(parts[0]);
    proxy.host = parts[1];
    proxy.port = std::stoi(parts[2]);
    
    if (parts.size() > 3) {
        proxy.username = parts[3];
    }
    if (parts.size() > 4) {
        proxy.password = parts[4];
    }
    
    return proxy;
}

bool ConfigParser::loadConfig(ProxyConfig& config) {
    std::ifstream file(config_path);
    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível abrir arquivo de configuração: " 
                  << config_path << std::endl;
        return false;
    }
    
    std::string line;
    bool in_proxy_list = false;
    
    while (std::getline(file, line)) {
        std::string trimmed = Utils::trim(line);
        
        // Verifica se entrou na seção [ProxyList]
        if (Utils::trim(Utils::toLower(trimmed)) == "[proxylist]") {
            in_proxy_list = true;
            continue;
        }
        
        if (in_proxy_list) {
            ProxyServer proxy = parseProxyLine(trimmed);
            if (!proxy.type.empty() && !proxy.host.empty() && proxy.port > 0) {
                config.proxy_list.push_back(proxy);
            }
        } else {
            parseLine(line, config);
        }
    }
    
    file.close();
    return true;
}

bool ConfigParser::saveConfig(const ProxyConfig& config) {
    std::ofstream file(config_path);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Arquivo de configuração MyProxy\n";
    file << "# Tipo de chain (dynamic_chain, strict_chain, random_chain, round_robin_chain)\n";
    file << "chain_type = " << config.chain_type << "\n\n";
    
    file << "# Comprimento da chain para modos random/round_robin\n";
    file << "chain_len = " << config.chain_len << "\n\n";
    
    file << "# Modo silencioso (sem output da biblioteca)\n";
    file << "quiet_mode = " << (config.quiet_mode ? "on" : "off") << "\n\n";
    
    file << "# Proxy DNS requests - sem vazamento de dados DNS\n";
    file << "proxy_dns = " << (config.proxy_dns ? "on" : "off") << "\n\n";
    
    file << "# Timeouts em milissegundos\n";
    file << "tcp_read_time_out = " << config.tcp_read_time_out << "\n";
    file << "tcp_connect_time_out = " << config.tcp_connect_time_out << "\n\n";
    
    file << "# Exclusões de localnet\n";
    for (const auto& net : config.localnet) {
        file << "localnet = " << net << "\n";
    }
    file << "\n";
    
    file << "# Configurações DNAT\n";
    for (const auto& dnat : config.dnat_rules) {
        file << "dnat = " << dnat << "\n";
    }
    file << "\n";
    
    file << "[ProxyList]\n";
    file << "# type    host            port    [user]  [pass]\n";
    file << "# -----   ------------    -----   ------  ------\n";
    
    for (const auto& proxy : config.proxy_list) {
        file << proxy.type << "   " << proxy.host << "   " << proxy.port;
        if (!proxy.username.empty()) {
            file << "   " << proxy.username;
            if (!proxy.password.empty()) {
                file << "   " << proxy.password;
            }
        }
        file << "\n";
    }
    
    file.close();
    return true;
}

void ConfigParser::setConfigPath(const std::string& path) {
    config_path = path;
}

std::string ConfigParser::getConfigPath() const {
    return config_path;
}
