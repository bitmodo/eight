#ifndef EIGHT_LEXER_H
#define EIGHT_LEXER_H

#include "api.h"

#include <istream>
#include <string>

namespace Eight {
    class Token;

    class Lexer {
        std::istream * m_in;

        std::string input;
        unsigned int p = 0;
        char c;
    public:
        // static const char EOF = (char) -1;
        static const int EOF_TYPE = 1;
        static const int FUNC = 2;
        static const int LPAREN = 3;
        static const int RPAREN = 4;
        static const int LBRACE = 5;
        static const int RBRACE = 6;
        static const int IDENTIFIER = 7;
        static const int STRING = 8;

        Lexer(std::istream *);

        void consume();

        void match(char);

        Token * func();

        Token * lparen();

        Token * rparen();

        Token * lbrace();

        Token * rbrace();

        Token * identifier();

        Token * string();
    };
}

#endif
