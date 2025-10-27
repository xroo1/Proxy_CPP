# ProxyCPP

**Ferramenta de Proxy em C++ para Redirecionamento de Tráfego de Rede**

## 📋 Sobre o Projeto

O ProxyCPP é uma ferramenta desenvolvida em C++ que permite executar qualquer aplicação através de uma cadeia de servidores proxy. Diferente de outras soluções, oferece um sistema de logs organizado onde cada aplicação tem seu próprio arquivo de log.

## 🎯 Diferencial Principal

**Sistema de Logs Separados por Aplicação** - Enquanto outras ferramentas mostram todos os logs misturados, o ProxyCPP cria logs individuais para cada aplicação. Isso significa que você pode monitorar o tráfego do weechat, curl, firefox e outras aplicações em arquivos separados, mantendo a organização e facilitando a análise.

## 🚀 Como Funciona

Sua Aplicação → ProxyCPP → Cadeia de Proxies → Internet
↓ ↓ ↓
Log próprio Log geral Redirecionamento
da app do sistema através de múltiplos proxies
text


## 💻 Instalação Rápida

```bash
# Clone o repositório
git clone https://github.com/seu-usuario/proxycpp
cd proxycpp

# Compile o projeto
make

# Teste a instalação
./myproxy --version

🛠 Como Usar
Sintaxe Básica
bash

./myproxy <aplicação> [argumentos]

Exemplos Práticos
bash

# Usar com weechat
./myproxy weechat

# Testar conexão com curl
./myproxy curl -s ifconfig.me

# Fazer download com wget
./myproxy wget http://exemplo.com/arquivo

# Navegar com firefox
./myproxy firefox
```

Configuração Personalizada
```bash

./myproxy --config /caminho/para/config.conf weechat
```
⚙️ Configura# ProxyCPP

**Ferramenta de Proxy em C++ para Redirecionamento de Tráfego de Rede**

## 📋 Sobre o Projeto

O ProxyCPP é uma ferramenta desenvolvida em C++ que permite executar qualquer aplicação através de uma cadeia de servidores proxy. Diferente de outras soluções, oferece um sistema de logs organizado onde cada aplicação tem seu próprio arquivo de log.

## 🎯 Diferencial Principal

**Sistema de Logs Separados por Aplicação** - Enquanto outras ferramentas mostram todos os logs misturados, o ProxyCPP cria logs individuais para cada aplicação. Isso significa que você pode monitorar o tráfego do weechat, curl, firefox e outras aplicações em arquivos separados, mantendo a organização e facilitando a análise.

## 🚀 Como Funciona

Sua Aplicação → ProxyCPP → Cadeia de Proxies → Internet
↓ ↓ ↓
Log próprio Log geral Redirecionamento
da app do sistema através de múltiplos proxies
text


## 💻 Instalação Rápida

```bash
# Clone o repositório
git clone https://github.com/seu-usuario/proxycpp
cd proxycpp

# Compile o projeto
make

# Teste a instalação
./myproxy --version

🛠 Como Usar
Sintaxe Básica
bash

./myproxy <aplicação> [argumentos]

Exemplos Práticos
bash

# Usar com weechat
./myproxy weechat

# Testar conexão com curl
./myproxy curl -s ifconfig.me

# Fazer download com wget
./myproxy wget http://exemplo.com/arquivo

# Navegar com firefox
./myproxy firefox

Configuração Personalizada

./myproxy --config /caminho/para/config.conf weechat
```

## ⚙️ Configuração

Crie o arquivo config/myproxy.conf:
ini

# Tipo de cadeia de proxies
chain_type = dynamic_chain

# Comprimento da cadeia
chain_len = 2

# Modo silencioso (reduz output no terminal)
quiet_mode = off

# Proxy de requisições DNS
proxy_dns = on

# Timeouts em milissegundos
tcp_read_time_out = 15000
tcp_connect_time_out = 8000

# Lista de Proxies
[ProxyList]
socks5 127.0.0.1 9050
socks4 127.0.0.1 9050
# http   192.168.1.100 8080 usuario senha

📊 Sistema de Logs
Estrutura de Arquivos
text

logs/
├── myproxy_general.log    # Log geral do sistema
├── weechat.log           # Log específico do weechat
├── curl.log              # Log específico do curl
├── firefox.log           # Log específico do firefox
└── wget.log              # Log específico do wget

Monitoramento em Tempo Real
```bash

# Ver todos os logs do sistema
tail -f logs/myproxy_general.log

# Ver apenas logs do weechat
tail -f logs/weechat.log

# Ver apenas logs do curl
tail -f logs/curl.log
```

🔗 Tipos de Cadeia de Proxies
dynamic_chain (Padrão)

    Tenta cada proxy em sequência até encontrar um que funcione

    Remove automaticamente proxies que falham

    Ideal para redes com proxies instáveis

strict_chain

    Usa todos os proxies na ordem exata da configuração

    Falha se qualquer proxy da cadeia estiver offline

    Máxima segurança e controle

random_chain

    Seleciona proxies aleatórios da lista

    Bom para distribuição de carga

    Aumenta o anonimato

round_robin_chain

    Alterna entre proxies de forma circular

    Balanceamento uniforme de tráfego

✅ Vantagens do ProxyCPP

    📁 Logs Organizados: Cada aplicação tem seu arquivo de log separado

    🎯 Fácil de Usar: Sintaxe simples: ./proxy aplicação

    ⚙️ Altamente Configurável: Arquivo de configuração intuitivo

    🔌 Multi-protocolo: Suporte a SOCKS4, SOCKS5, HTTP

    🚀 Performance: Desenvolvido em C++ para melhor desempenho

    🔒 Segurança: Proteção contra vazamento de DNS

🐛 Solução de Problemas
Problema: Permissão de Arquivos de Log
bash

# Execute como superusuário para logs em /var/log/
sudo ./myproxy weechat

# Ou use a pasta local de logs
mkdir -p logs
./myproxy weechat

Problema: Proxy Não Conecta
bash

# Verifique se o serviço proxy está rodando
sudo systemctl status tor

# Teste a conexão manualmente
curl --socks5 127.0.0.1:9050 -s ifconfig.me

Problema: Logs Muito Detalhados
ini

# No arquivo myproxy.conf, altere para:
quiet_mode = on

Problema: Conexões Lentas
ini

# Aumente os timeouts no arquivo de configuração
tcp_read_time_out = 30000
tcp_connect_time_out = 15000ção

Crie o arquivo config/myproxy.conf:
ini

# Tipo de cadeia de proxies
chain_type = dynamic_chain

# Comprimento da cadeia
chain_len = 2

# Modo silencioso (reduz output no terminal)
quiet_mode = off

# Proxy de requisições DNS
proxy_dns = on

# Timeouts em milissegundos
tcp_read_time_out = 15000
tcp_connect_time_out = 8000

# Lista de Proxies
[ProxyList]
socks5 127.0.0.1 9050
socks4 127.0.0.1 9050
# http   192.168.1.100 8080 usuario senha

📊 Sistema de Logs
Estrutura de Arquivos
text

logs/
├── myproxy_general.log    # Log geral do sistema
├── weechat.log           # Log específico do weechat
├── curl.log              # Log específico do curl
├── firefox.log           # Log específico do firefox
└── wget.log              # Log específico do wget

Monitoramento em Tempo Real
bash

# Ver todos os logs do sistema
tail -f logs/myproxy_general.log

# Ver apenas logs do weechat
tail -f logs/weechat.log

# Ver apenas logs do curl
tail -f logs/curl.log

🔗 Tipos de Cadeia de Proxies
dynamic_chain (Padrão)

    Tenta cada proxy em sequência até encontrar um que funcione

    Remove automaticamente proxies que falham

    Ideal para redes com proxies instáveis

strict_chain

    Usa todos os proxies na ordem exata da configuração

    Falha se qualquer proxy da cadeia estiver offline

    Máxima segurança e controle

random_chain

    Seleciona proxies aleatórios da lista

    Bom para distribuição de carga

    Aumenta o anonimato

round_robin_chain

    Alterna entre proxies de forma circular

    Balanceamento uniforme de tráfego

✅ Vantagens do ProxyCPP

    📁 Logs Organizados: Cada aplicação tem seu arquivo de log separado

    🎯 Fácil de Usar: Sintaxe simples: ./proxy aplicação

    ⚙️ Altamente Configurável: Arquivo de configuração intuitivo

    🔌 Multi-protocolo: Suporte a SOCKS4, SOCKS5, HTTP

    🚀 Performance: Desenvolvido em C++ para melhor desempenho

    🔒 Segurança: Proteção contra vazamento de DNS

🐛 Solução de Problemas
Problema: Permissão de Arquivos de Log
bash

# Execute como superusuário para logs em /var/log/
sudo ./myproxy weechat

# Ou use a pasta local de logs
mkdir -p logs
./myproxy weechat

Problema: Proxy Não Conecta
bash

# Verifique se o serviço proxy está rodando
sudo systemctl status tor

# Teste a conexão manualmente
curl --socks5 127.0.0.1:9050 -s ifconfig.me

Problema: Logs Muito Detalhados
ini

# No arquivo myproxy.conf, altere para:
quiet_mode = on

Problema: Conexões Lentas
ini

# Aumente os timeouts no arquivo de configuração
tcp_read_time_out = 30000
tcp_connect_time_out = 15000
