#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {
    // Split string por delimitador
    std::vector<std::string> split(const std::string& str, char delimiter);
    
    // Trim whitespace
    std::string trim(const std::string& str);
    
    // Verifica se string começa com prefixo
    bool startsWith(const std::string& str, const std::string& prefix);
    
    // Verifica se string contém substring
    bool contains(const std::string& str, const std::string& substr);
    
    // Converte para lowercase
    std::string toLower(const std::string& str);
    
    // Obtém timestamp formatado
    std::string getTimestamp();
    
    // Cria diretório se não existir
    bool createDirectory(const std::string& path);
}

#endif
