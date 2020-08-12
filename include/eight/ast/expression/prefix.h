#ifndef EIGHT_AST_EXPRESSION_PREFIX_H
#define EIGHT_AST_EXPRESSION_PREFIX_H

#include "eight/api.h"
#include "eight/ast.h"

namespace Eight {
    class PostfixExpression;

    class PrefixExpression : public AST {
        PostfixExpression *m_expression;
    public:
        PrefixExpression(PostfixExpression*);
        virtual ~PrefixExpression() override;

        PrefixExpression(const PrefixExpression&) = delete;
        PrefixExpression &operator=(const PrefixExpression&) = delete;
        PrefixExpression(PrefixExpression&&);
        PrefixExpression &operator=(PrefixExpression&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const PostfixExpression *const expression() const {
            return m_expression;
        }
    };
}

#endif
