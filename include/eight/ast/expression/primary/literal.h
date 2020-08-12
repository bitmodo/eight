#ifndef EIGHT_AST_EXPRESSION_PRIMARY_LITERAL_H
#define EIGHT_AST_EXPRESSION_PRIMARY_LITERAL_H

#include "eight/ast/expression/primary.h"

namespace Eight {
    class Literal : public PrimaryExpression {
    public:
        virtual ~Literal() override = default;

        virtual void analyze(Sema*) override = 0;
        virtual llvmValue accept(Visitor*) const override = 0;
    };
}

#endif
