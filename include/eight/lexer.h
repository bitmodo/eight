#ifndef EIGHT_LEXER_H
#define EIGHT_LEXER_H

#include "api.h"

#include <istream>
#include <string>
#include <stack>

namespace Eight {
    class Token;

    struct FilePos {
        unsigned m_pos = 0;
        unsigned m_line = 0;
        unsigned m_col = 0;
    public:
        FilePos() = default;

        const unsigned &position() const { return m_pos; }
        unsigned &position() { return m_pos; }

        const unsigned &line() const { return m_line; }
        unsigned &line() { return m_line; }

        const unsigned &column() const { return m_col; }
        unsigned &column() { return m_col; }
    };

    class Lexer {
        std::istream * m_in;
        std::stack<FilePos> m_posStack;

        std::string input;
        FilePos pos;
        char c = '\0';
        bool s = true;
    public:
        Lexer(std::istream *);
        ~Lexer();

        inline const FilePos &position() const {
            return pos;
        }

        void push();
        void pop();
        void popSeek();

        void seek(std::streampos);

        char consume();
        char consumeStart();
        const std::string consume(unsigned);

        const std::string look(unsigned = 1);
        const std::string look(unsigned, unsigned);

        bool consumeIf(char);
        bool consumeIf(const std::string&);

        const std::string cleanWhitespace(const std::string&) const;

        bool eof();
        bool newLine();

        bool func();
        bool structure();
        bool ret();

        bool let();
        bool var();

        bool equals();
        bool colon();
        bool semicolon();
        bool comma();
        bool dot();
        bool elipses();

        bool lparen();
        bool rparen();

        bool lbrace();
        bool rbrace();

        bool doubleQuote();

        Token * identifier();

        Token * string();
    };
}

#endif
