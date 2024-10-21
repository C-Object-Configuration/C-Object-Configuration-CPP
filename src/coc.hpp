#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace coc {
    class Structure;
    std::optional<Structure> Open(std::string path);
    std::optional<Structure> Load(std::string data);

    #define TYPE(type, name)                                    \
    class name {                                                \
    friend std::optional<Structure> Load(std::string data);     \
    public:                                                     \
        std::optional<type> Get(std::string_view key);          \
        std::optional<type> operator[](std::string_view key);   \
        std::unordered_map<std::string, type> map;              \
    } name;

    class Structure {
    friend std::optional<Structure> Load(std::string data);
    public:
        TYPE(coc::Structure, Struct)
        TYPE(bool, Bool)
        TYPE(char, Char)
        TYPE(int, Int)
        TYPE(long long int, Long)
        TYPE(float, Float)
        TYPE(double, Double)
        TYPE(std::string, String)

        struct Array {
            TYPE(std::vector<coc::Structure>, Struct)
            TYPE(std::vector<bool>, Bool)
            TYPE(std::vector<char>, Char)
            TYPE(std::vector<int>, Int)
            TYPE(std::vector<long long int>, Long)
            TYPE(std::vector<float>, Float)
            TYPE(std::vector<double>, Double)
            TYPE(std::vector<std::string>, String)
        } Array;

    private:
        enum tokenizeValueCallback {
            Fail,
            IsSingle,
            IsArray
        };
        tokenizeValueCallback tokenize(std::string_view data, int &index, std::string stringedType, std::string &key, std::vector<std::string> &values, bool(*hasValue)(std::string_view data, int &index, bool isArray, std::string &key, std::string &value));
        bool tokenizeType(std::string_view data, int &index, std::string &type);
        bool tokenizeKey(std::string_view data, int &index, std::string &key);
        tokenizeValueCallback tokenizeValue(std::string_view data, int &index, std::string &key, std::vector<std::string> &values, bool(*hasValue)(std::string_view data, int &index, bool isArray, std::string &key, std::string &value));
    };
}