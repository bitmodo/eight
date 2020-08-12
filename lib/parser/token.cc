#include "eight/parser/token.h"

Eight::Token::Token(unsigned start, const std::string& text) : m_start(start), m_text(text) {}
