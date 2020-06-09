#ifndef EIGHT_PARSER_H
#define EIGHT_PARSER_H

#include "api.h"

#ifdef __cplusplus

#include <istream>
#include <string>

namespace Eight {
    class AST;

    class Token;

    struct Parse {
        const std::string m_name;
        const std::istream * m_in;

        std::string m_file;
    public:
        Parse(const std::string, const std::istream *);

        inline const std::string& name() const {
            return m_name;
        }
    };

    Parse * parse(const std::string, const std::istream *);
    Parse * parseFile(const std::string);
}

#  define EIGHT_PARSER Eight::Parse

#else

struct Parse;

#  define EIGHT_PARSER Parse

#endif

HEDLEY_C_DECL EIGHT_API EIGHT_PARSER * ParseEightFile(const char *);

#endif
