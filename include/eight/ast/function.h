#ifndef EIGHT_AST_FUNCTION_H
#define EIGHT_AST_FUNCTION_H

#include <string>

#include "eight/ast.h"

namespace Eight {
    class Function : public AST {
        const std::string m_name;
    public:
        inline const std::string name(void) const {
            return m_name;
        }
    };
}

#endif
