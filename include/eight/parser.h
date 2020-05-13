#ifndef EIGHT_PARSER_PARSER_H
#define EIGHT_PARSER_PARSER_H

#include <istream>
#include <string>

namespace Eight {
    class AST;

    namespace Parser {
        class Token;
    }

    struct Parse {
        std::string name;
        std::istream in;

        std::string file;
    public:
        std::string name(void);

        std::string find(Parser::Token);
    };

    Parse * parse(std::string, std::istream);
    Parse * parseFile(std::string);
}

#endif
