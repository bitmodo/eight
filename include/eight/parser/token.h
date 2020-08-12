#ifndef EIGHT_PARSER_TOKEN_H
#define EIGHT_PARSER_TOKEN_H

#include "eight/api.h"

#include <string>

namespace Eight {
    class Token {
        const unsigned m_start;
        const std::string m_text;
    public:
        Token(unsigned, const std::string&);

        inline const unsigned start() const noexcept {
            return m_start;
        }

        inline const unsigned length() const noexcept {
            return m_text.length();
        }

        inline const std::string& text() const noexcept {
            return m_text;
        }
    };
}

#endif
