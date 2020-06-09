#include "eight/lexer.h"
#include "eight/parser/token.h"

using namespace std;
using namespace Eight;

Lexer::Lexer(std::istream * in) : m_in(move(in)) {}

void Lexer::consume() {
    this->p++;

    if (m_in->eof()) {
        this->c = '\0';
    } else {
        this->c = this->m_in->get();
    }
}

void Lexer::match(char) {

}

Token * Lexer::func() {
    return NULL;
}

Token * Lexer::lparen() {
    return NULL;
}

Token * Lexer::rparen() {
    return NULL;
}

Token * Lexer::lbrace() {
    return NULL;
}

Token * Lexer::rbrace() {
    return NULL;
}

Token * Lexer::identifier() {
    return NULL;
}

Token * Lexer::string() {
    return NULL;
}
