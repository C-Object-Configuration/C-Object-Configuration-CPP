#include "coc.hpp"

coc::Structure::tokenizeValueCallback coc::Structure::tokenize(std::string_view data, int &index, std::string stringedType, std::string &key, std::vector<std::string> &values, bool(*hasValue)(std::string_view data, int &index, bool isArray, std::string &key, std::string &value)) {
    if (!tokenizeType(data, index, stringedType) || !tokenizeKey(data, index, key)) return Fail;
    return tokenizeValue(data, index, key, values, hasValue);
}

bool coc::Structure::tokenizeType(std::string_view data, int &index, std::string &type) {
    switch (data[index-1]) {
        default: return false;
        case ' ':
        case '{':
        case '}':
        case '"':
        case '\'':
            break;
    }

    type += ' ';

    for (char c : type) {
        if (c != data[index++]) return false;
    }

    return true;
}

bool coc::Structure::tokenizeKey(std::string_view data, int &index, std::string &key) {
    do {
        if (data[index] == ' ') {
            if (!key.empty()) return true;
            continue;
        }

        if (data[index] == '=' && !key.empty()) {
            index--;
            return true;
        }

        key += data[index];
    } while (index++ < data.length());
    return false;
}

coc::Structure::tokenizeValueCallback coc::Structure::tokenizeValue(std::string_view data, int &index, std::string &key, std::vector<std::string> &values, bool(*hasValue)(std::string_view data, int &index, bool isArray, std::string &key, std::string &value)) {
    bool hasEqual = false;

    while (index++ < data.length()) {
        switch (data[index]) {
            case ' ': continue;

            case '=': {
                if (hasEqual) return Fail;
                hasEqual = true;
                continue;
            }

            case '[': {
                if (!hasEqual) return Fail;
                goto parseArray;
            }

            default: {
                if (!hasEqual) return Fail;
                goto parseSingle;
            }
        }
    }

    parseSingle:
    values.emplace_back("");
    return hasValue(data, index, false, key, values[0])
        ? IsSingle
        : Fail
    ;

    parseArray:
    bool hasComma = true;
    while (index++ < data.length()) {
        switch (data[index]) {
            case ' ': continue;

            case ',': {
                if (hasComma) return Fail;
                hasComma = true;
                continue;
            }

            case ']': {
                return hasComma
                    ? Fail
                    : IsArray
                ;
            }

            default: {
                if (!hasComma) return Fail;
                hasComma = false;
                values.emplace_back("");
                if (!hasValue(data, index, true, key, values[values.size()-1])) {
                    return Fail;
                }
                continue;
            }
        }
    }
    return Fail;
}