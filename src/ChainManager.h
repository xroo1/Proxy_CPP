#ifndef CHAIN_MANAGER_H
#define CHAIN_MANAGER_H

#include "ConfigParser.h"
#include "Logger.h"
#include <vector>
#include <random>

class ChainManager {
private:
    ProxyConfig config;
    std::shared_ptr<Logger> logger;
    
    // Para modos random e round_robin
    std::random_device rd;
    std::mt19937 rng;
    size_t round_robin_index;

public:
    ChainManager(const ProxyConfig& cfg, std::shared_ptr<Logger> log);
    
    // Seleciona chain baseado no tipo
    std::vector<ProxyServer> selectChain();
    
    // Diferentes tipos de chain
    std::vector<ProxyServer> getDynamicChain();
    std::vector<ProxyServer> getStrictChain();
    std::vector<ProxyServer> getRandomChain();
    std::vector<ProxyServer> getRoundRobinChain();
    
    // Verifica se IP está em localnet (não deve usar proxy)
    bool isLocalnet(const std::string& ip);
    
    // Aplica regras DNAT
    bool applyDNAT(std::string& host, int& port);
};

#endif
