#include "coc.hpp"
#include <fstream>

std::optional<coc::Structure> coc::Open(std::string path) {
    std::ifstream file;
    file.open(path);
    if (file.fail()) return {};

    std::string data;
    char c;
    while (file.get(c)) {
        if (c == '\n') c = ' ';
        data += c;
    }

    std::optional<coc::Structure> structure = coc::Load(data);
    return structure
        ? std::move(structure)
        : std::nullopt
    ;
}