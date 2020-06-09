#ifndef EIGHT_PARSER_TOKEN_H
#define EIGHT_PARSER_TOKEN_H

#include <string>

namespace Eight {
    class Token {
        int m_type;
        std::string m_text;
    public:
        Token(int, std::string);

        inline const int& type() const {
            return m_type;
        }

        inline const std::string& text() const {
            return m_text;
        }
    };
}

#endif
