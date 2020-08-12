#ifndef EIGHT_EXCEPTION_H
#define EIGHT_EXCEPTION_H

#include <string>
#include <sstream>
#include <exception>

namespace Eight {
    class syntax_error : public std::exception {
        const char *m_message;
        unsigned m_line, m_column;
    public:
        syntax_error(unsigned line, unsigned column, const char *message) : m_message(message), m_line(line), m_column(column) {}

        virtual const char *what() const throw() override {
            return "Syntax error";
        }

        const std::string message() const throw() {
            std::stringstream str;
            str << "Syntax error on " << m_line << ':' << m_column << ": " << m_message;
            return str.str();
        }
    };
}

#endif
