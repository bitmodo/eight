#ifndef EIGHT_AST_EXPRESSION_POSTFIX_H
#define EIGHT_AST_EXPRESSION_POSTFIX_H

#include "eight/api.h"
#include "eight/ast.h"

namespace Eight {
    class PostfixExpression : public AST {
    public:
        virtual ~PostfixExpression() override = default;

        virtual void analyze(Sema*) override = 0;
        virtual llvmValue accept(Visitor*) const override = 0;

        virtual void setHead(PostfixExpression*) = 0;
    };
}

#endif
