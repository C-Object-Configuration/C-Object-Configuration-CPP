#include "coc.hpp"

#define GETTER(type, name)                                                                      \
std::optional<type> coc::Structure::name::Get(std::string_view key) {                           \
    if (!map.contains(key.data())) return {};                                                   \
    return map.at(key.data());                                                                  \
}                                                                                               \
std::optional<type> coc::Structure::name::operator[](std::string_view key) { return Get(key); }

#define GETTERS(type, name)             \
GETTER(type, name)                      \
GETTER(std::vector<type>, Array::name)

GETTERS(coc::Structure, Struct)
GETTERS(bool, Bool)
GETTERS(char, Char)
GETTERS(int, Int)
GETTERS(long long int, Long)
GETTERS(float, Float)
GETTERS(double, Double)
GETTERS(std::string, String)