#include "Util.hpp"

#include <fstream>
#include <sstream>

namespace ws {
    auto load_page(const std::string& path) -> std::string {
        std::ifstream file(path);
        if (!file.is_open()) {
            return "";
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
}