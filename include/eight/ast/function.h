#ifndef EIGHT_AST_FUNCTION_H
#define EIGHT_AST_FUNCTION_H

#include <string>

#include "eight/ast.h"

namespace Eight {
    namespace AST {
        class Function : public AST {
            std::string name;
        public:
            std::string name(void);
        };
    }
}

#endif
