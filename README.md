# Proxy_CPP

**Ferramenta avançada de redirecionamento de tráfego em C++**

---

## **O que é o Proxy_CPP?**

```bash
# Nome Técnico: Proxy Client com Chain Management
# Tipo: SOCKS/HTTP Proxy Redirector  
# Inspiração: Proxychains (implementação em C++)


┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Sua App   │ -> │   Proxy 1   │ -> │   Proxy 2   │ -> │   Internet  │
│  (weechat)  │    │ (127.0.0.1) │    │  (tor)      │    │             │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘


# 1. Clone e compile
git clone https://github.com/seu-usuario/proxy_cpp.git
cd proxy_cpp
make

# 2. Teste a instalação
./myproxy --version

# ========================
# CONFIGURAÇÃO PRINCIPAL
# ========================

# Tipo de chain (dynamic_chain, strict_chain, random_chain, round_robin_chain)
chain_type = dynamic_chain

# Comprimento da chain para modos random/round_robin
chain_len = 2

# Modo silencioso (sem output no console)
quiet_mode = off

# Proxy DNS requests - evita vazamento de DNS
proxy_dns = on

# Timeouts em milissegundos
tcp_read_time_out = 15000
tcp_connect_time_out = 8000

# ========================
# REGRAS DE REDE
# ========================

# Exclusões de localnet (não passam pelo proxy)
localnet = 192.168.1.0/255.255.255.0
localnet = 127.0.0.0/255.0.0.0

# Regras DNAT (redirecionamento)
# dnat = ip_origem:porta ip_destino:porta
# dnat = 1.1.1.1:443 1.1.1.2:443

# ========================
# LISTA DE PROXIES
# ========================

[ProxyList]
# type    host            port    [user]  [pass]
# -----   ------------    -----   ------  ------
socks5   127.0.0.1       9050
socks4   127.0.0.1       9050
# http     192.168.1.100  8080    user    pass






