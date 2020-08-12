#ifndef EIGHT_AST_EXPRESSION_PRIMARY_H
#define EIGHT_AST_EXPRESSION_PRIMARY_H

#include "eight/api.h"
#include "eight/ast.h"

namespace Eight {
    class PrimaryExpression : public AST {
    public:
        virtual ~PrimaryExpression() override = default;

        virtual void analyze(Sema*) override = 0;
        virtual llvmValue accept(Visitor*) const override = 0;
    };
}

#endif
