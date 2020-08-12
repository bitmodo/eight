#ifndef EIGHT_AST_STATEMENT_EXPRESSION_H
#define EIGHT_AST_STATEMENT_EXPRESSION_H

#include "eight/ast/statement.h"

namespace Eight {
    class Expression;

    class ExpressionStatement : public Statement {
        Expression *m_expression;
    public:
        ExpressionStatement(Expression*);
        ~ExpressionStatement() override;

        ExpressionStatement(const ExpressionStatement&) = delete;
        ExpressionStatement &operator=(const ExpressionStatement&) = delete;
        ExpressionStatement(ExpressionStatement&&);
        ExpressionStatement &operator=(ExpressionStatement&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const Expression *const expression() const {
            return m_expression;
        }

        // inline Expression *expression() {
        //     return m_expression;
        // }
    };
}

#endif
