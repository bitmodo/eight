#ifndef EIGHT_AST_EXPRESSION_BINARY_H
#define EIGHT_AST_EXPRESSION_BINARY_H

#include "eight/api.h"
#include "eight/ast.h"

namespace Eight {
    class BinaryExpression : public AST {
        
    public:
        virtual ~BinaryExpression() override = default;

        virtual void analyze(Sema*) override = 0;
        virtual llvmValue accept(Visitor*) const override = 0;
    };
}

#endif
