#ifndef EIGHT_AST_EXPRESSION_POSTFIX_PRIMARY_H
#define EIGHT_AST_EXPRESSION_POSTFIX_PRIMARY_H

#include "eight/ast/expression/postfix.h"

namespace Eight {
    class PrimaryExpression;

    class PrimaryPostfixExpression : public PostfixExpression {
        PrimaryExpression *m_expression;
    public:
        PrimaryPostfixExpression(PrimaryExpression*);
        ~PrimaryPostfixExpression() override;

        PrimaryPostfixExpression(const PrimaryPostfixExpression&) = delete;
        PrimaryPostfixExpression &operator=(const PrimaryPostfixExpression&) = delete;
        PrimaryPostfixExpression(PrimaryPostfixExpression&&);
        PrimaryPostfixExpression &operator=(PrimaryPostfixExpression&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        virtual void setHead(PostfixExpression*) override {}

        inline const PrimaryExpression *const expression() const {
            return m_expression;
        }
    };
}

#endif
