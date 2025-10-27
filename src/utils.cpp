#include "utils.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <sys/stat.h>

namespace Utils {
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        
        while (std::getline(ss, token, delimiter)) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }
        
        return tokens;
    }
    
    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        
        if (start == std::string::npos) {
            return "";
        }
        
        return str.substr(start, end - start + 1);
    }
    
    bool startsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() && 
               str.compare(0, prefix.size(), prefix) == 0;
    }
    
    bool contains(const std::string& str, const std::string& substr) {
        return str.find(substr) != std::string::npos;
    }
    
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
    std::string getTimestamp() {
        time_t now = time(nullptr);
        tm* local_time = localtime(&now);
        
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
        return std::string(buffer);
    }
    
    bool createDirectory(const std::string& path) {
        return system(("mkdir -p " + path).c_str()) == 0;
    }
}
