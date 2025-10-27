#include "ChainManager.h"
#include "utils.h"
#include <algorithm>

ChainManager::ChainManager(const ProxyConfig& cfg, std::shared_ptr<Logger> log) 
    : config(cfg), logger(log), rng(rd()), round_robin_index(0) {
}

std::vector<ProxyServer> ChainManager::selectChain() {
    std::string chain_type = Utils::toLower(config.chain_type);
    
    logger->debug("Selecionando chain do tipo: " + chain_type);
    
    if (chain_type == "strict_chain") {
        return getStrictChain();
    }
    else if (chain_type == "random_chain") {
        return getRandomChain();
    }
    else if (chain_type == "round_robin_chain") {
        return getRoundRobinChain();
    }
    else { // dynamic_chain padrão
        return getDynamicChain();
    }
}

std::vector<ProxyServer> ChainManager::getDynamicChain() {
    // No dynamic chain, tenta usar a lista completa
    // Na implementação real, você testaria cada proxy e removeria os que falham
    return config.proxy_list;
}

std::vector<ProxyServer> ChainManager::getStrictChain() {
    // Usa todos os proxies na ordem definida
    return config.proxy_list;
}

std::vector<ProxyServer> ChainManager::getRandomChain() {
    std::vector<ProxyServer> chain = config.proxy_list;
    
    // Embaralha a lista
    std::shuffle(chain.begin(), chain.end(), rng);
    
    // Pega apenas chain_len proxies
    if (config.chain_len > 0 && config.chain_len < chain.size()) {
        chain.resize(config.chain_len);
    }
    
    return chain;
}

std::vector<ProxyServer> ChainManager::getRoundRobinChain() {
    std::vector<ProxyServer> chain;
    
    if (config.proxy_list.empty()) {
        return chain;
    }
    
    // Seleciona proxies começando do índice atual
    for (size_t i = 0; i < config.proxy_list.size() && 
         (config.chain_len == 0 || i < static_cast<size_t>(config.chain_len)); i++) {
        size_t index = (round_robin_index + i) % config.proxy_list.size();
        chain.push_back(config.proxy_list[index]);
    }
    
    // Avança índice para próxima vez
    round_robin_index = (round_robin_index + 1) % config.proxy_list.size();
    
    return chain;
}

bool ChainManager::isLocalnet(const std::string& ip) {
    // Implementação simplificada - na prática precisa verificar sub-redes
    for (const auto& net : config.localnet) {
        if (Utils::contains(net, ip)) {
            return true;
        }
    }
    
    // Verifica se é localhost
    return ip == "127.0.0.1" || ip == "localhost";
}

bool ChainManager::applyDNAT(std::string& host, int& port) {
    for (const auto& rule : config.dnat_rules) {
        auto parts = Utils::split(rule, ' ');
        if (parts.size() == 2) {
            auto src_parts = Utils::split(parts[0], ':');
            auto dst_parts = Utils::split(parts[1], ':');
            
            if (src_parts.size() == 2 && dst_parts.size() == 2) {
                std::string src_host = src_parts[0];
                int src_port = std::stoi(src_parts[1]);
                
                if ((host == src_host || src_host == "*") && 
                    (port == src_port || src_port == 0)) {
                    host = dst_parts[0];
                    port = std::stoi(dst_parts[1]);
                    return true;
                }
            }
        }
    }
    return false;
}
