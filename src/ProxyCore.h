#ifndef PROXY_CORE_H
#define PROXY_CORE_H

#include "ConfigParser.h"
#include "ChainManager.h"
#include "Logger.h"
#include <string>
#include <thread>
#include <atomic>

struct ConnectionInfo {
    int client_socket;
    std::string client_ip;
    std::string app_name;
    std::string dest_host;
    int dest_port;
    time_t start_time;
    std::string chain_info;

    ConnectionInfo(int sock, const std::string& ip, const std::string& app, 
                   const std::string& host, int port, const std::string& chain);
};

class ProxyCore {
private:
    ProxyConfig config;
    ChainManager chain_manager;
    std::shared_ptr<Logger> logger;
    
    std::atomic<bool> running;
    int listen_port;
    
    // Mapa de conexões ativas
    std::unordered_map<int, std::shared_ptr<ConnectionInfo>> active_connections;
    std::mutex connections_mutex;

    // Métodos internos
    std::string detectAppName(const std::string& client_ip, const std::string& cmd_line);
    void addActiveConnection(const ConnectionInfo& conn_info);
    void removeActiveConnection(int client_socket);
    void logConnectionSummary(const ConnectionInfo& conn_info, 
                             size_t bytes_sent, size_t bytes_received);
    
    // Conexão através da chain
    int connectThroughChain(const std::vector<ProxyServer>& chain, 
                           const std::string& target_host, int target_port,
                           const std::string& app_name);

public:
    ProxyCore(const ProxyConfig& cfg, std::shared_ptr<Logger> log);
    ~ProxyCore();
    
    bool initialize();
    void start(int port = 1080);
    void stop();
    
    // Executa comando através do proxy
    int executeThroughProxy(const std::string& command, const std::vector<std::string>& args);
};

#endif
