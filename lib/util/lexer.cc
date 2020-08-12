#include "eight/util/lexer.h"



bool Eight::util::whitespace(char x) {
    return x == ' ' || x == '\t' || x == '\n' || x == '\r';
}

bool Eight::util::letter(char x) {
    return ('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z');
}

bool Eight::util::letters(const std::string& str) {
    for (const char& x : str)
        if (HEDLEY_UNLIKELY(!letter(x)))
            return false;
    
    return true;
}

bool Eight::util::number(char x) {
    return '0' <= x && x <= '9';
}

bool Eight::util::numbers(const std::string& str) {
    for (const char& x : str)
        if (HEDLEY_UNLIKELY(!number(x)))
            return false;
    
    return true;
}

bool Eight::util::alphanumeric(char x) {
    return letter(x) || number(x);
}

bool Eight::util::alphanumeric(const std::string& str) {
    for (const char& x : str)
        if (HEDLEY_UNLIKELY(!alphanumeric(x)))
            return false;
    
    return true;
}
