#ifndef EIGHT_UTIL_LEXER_H
#define EIGHT_UTIL_LEXER_H

#include "eight/api.h"

#include <string>

namespace Eight {
    namespace util {
        HEDLEY_CONST bool whitespace(char);

        HEDLEY_CONST bool letter(char);
        HEDLEY_CONST bool letters(const std::string&);

        HEDLEY_CONST bool number(char);
        HEDLEY_CONST bool numbers(const std::string&);

        HEDLEY_CONST bool alphanumeric(char);
        HEDLEY_CONST bool alphanumeric(const std::string&);
    }
}

#endif
