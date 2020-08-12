#include "eight/lexer.h"
#include "eight/parser/token.h"
#include "eight/util/lexer.h"

#include <iostream>

using namespace std;
using namespace Eight;

Lexer::Lexer(std::istream * in) : m_in(std::move(in)) {}

Lexer::~Lexer() {
    delete m_in;
}

void Lexer::push() {
    m_posStack.push(pos);
}

void Lexer::pop() {
    m_posStack.pop();
}

void Lexer::popSeek() {
    const FilePos tmp = m_posStack.top();
    
    seek(tmp.position());
    pos = tmp;

    pop();
}

void Lexer::seek(std::streampos pos) {
    if (!m_in->good()) {
        m_in->clear();
    }

    m_in->seekg(pos);
    s = m_in->good();
}

char Lexer::consume() {
    if (s) {
        c = m_in->get();

        pos.position()++;
        if (c == '\n' || c == '\r') {
            pos.line()++;
            pos.column() = 0;
        } else {
            pos.column()++;
        }
    } else {
        c = '\0';
    }

    s = m_in->good();
    return c;
}

char Lexer::consumeStart() {
    char x;

    do {
        x = consume();
    } while (HEDLEY_UNLIKELY(util::whitespace(x)));

    return x;
}

const std::string Lexer::consume(unsigned count) {
    std::string str;
    str = consumeStart();

    for (unsigned i = 1; i < count; i++)
        str += consume();

    return str;
}

const std::string Lexer::look(unsigned count) {
    return look(count, 0);
}

const std::string Lexer::look(unsigned count, unsigned offset) {
    const FilePos tmp_p = pos;
    seek(pos.position()+offset);

    std::string str = consume(count);

    seek(tmp_p.position());
    pos = tmp_p;

    return str;
}

bool Lexer::consumeIf(char x) {
    if (look().front() != x)
        return false;

    consumeStart();
    return true;
}

bool Lexer::consumeIf(const std::string& x) {
    if (look(x.length()).compare(x) != 0)
        return false;
    
    consume(x.length());
    return true;
}

const std::string Lexer::cleanWhitespace(const std::string& x) const {
    unsigned index = 0;

    while (util::whitespace(x.at(index))) {
        index++;
    }

    return x.substr(index, x.length()-index);
}

bool Lexer::eof() {
    const FilePos tmp_p = pos;

    char x;
    do {
        x = consume();
    } while (HEDLEY_UNLIKELY(util::whitespace(x)));

    const bool end = m_in->eof();

    seek(tmp_p.position());
    pos = tmp_p;

    return end;
}

bool Lexer::newLine() {
    const FilePos tmp_p = pos;

    char x = consume();
    if (HEDLEY_LIKELY(x == '\n'))
        return true;
    
    if (HEDLEY_LIKELY(x == '\r')) {
        if (HEDLEY_UNLIKELY(consume() != '\n')) {
            seek(tmp_p.position()+1);
            pos = tmp_p;
            pos.position()++;
        }

        return true;
    }

    seek(tmp_p.position());
    pos = tmp_p;

    return false;
}

bool Lexer::func() {
    return consumeIf("func");
}

bool Lexer::structure() {
    return consumeIf("struct");
}

bool Lexer::ret() {
    return consumeIf("return");
}

bool Lexer::let() {
    return consumeIf("let");
}

bool Lexer::var() {
    return consumeIf("var");
}

bool Lexer::equals() {
    return consumeIf('=');
}

bool Lexer::colon() {
    return consumeIf(':');
}

bool Lexer::semicolon() {
    return consumeIf(';');
}

bool Lexer::comma() {
    return consumeIf(',');
}

bool Lexer::dot() {
    return consumeIf('.');
}

bool Lexer::elipses() {
    return consumeIf("...");
}

bool Lexer::lparen() {
    return consumeIf('(');
}

bool Lexer::rparen() {
    return consumeIf(')');
}

bool Lexer::lbrace() {
    return consumeIf('{');
}

bool Lexer::rbrace() {
    return consumeIf('}');
}

bool Lexer::doubleQuote() {
    return consumeIf('"');
}

Token * Lexer::identifier() {
    if (HEDLEY_UNLIKELY(!util::alphanumeric(look())))
        return nullptr;
    
    const unsigned start = pos.position();
    std::string str;
    str = consumeStart();

    while (HEDLEY_LIKELY(util::alphanumeric(m_in->peek()))) {
        str += consume();
    }

    return new Token(start, str);
}

#ifdef STRING_ESCAPE_CASE
#  undef STRING_ESCAPE_CASE
#endif
#define STRING_ESCAPE_CASE(c, e) case c: consume(2); str += e; break

Token * Lexer::string() {
    if (HEDLEY_UNLIKELY(!doubleQuote()))
        return nullptr;
    
    const unsigned start = pos.position();
    std::string str;

    while (HEDLEY_LIKELY(look().front() != '"')) {
        if (look().front() == '\\') {
            std::string sequence = look(2);
            char c = sequence.at(1);

            switch (c) {
                STRING_ESCAPE_CASE('a', '\a');
                STRING_ESCAPE_CASE('b', '\b');
                STRING_ESCAPE_CASE('f', '\f');
                STRING_ESCAPE_CASE('n', '\n');
                STRING_ESCAPE_CASE('r', '\r');
                STRING_ESCAPE_CASE('t', '\t');
                STRING_ESCAPE_CASE('v', '\v');
                STRING_ESCAPE_CASE('\\', '\\');
                STRING_ESCAPE_CASE('\'', '\'');
                STRING_ESCAPE_CASE('"', '\"');
                STRING_ESCAPE_CASE('?', '\?');
                default:
                    consume();
                    cerr << "Unknown escape sequence" << endl;
                    break;
            }
        } else
            str += consume();
    }

    consume();

    return new Token(start, str);
}
