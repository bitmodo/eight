#ifndef EIGHT_AST_EXPRESSION_H
#define EIGHT_AST_EXPRESSION_H

#include "eight/api.h"
#include "eight/ast.h"

#include <vector>

namespace Eight {
    class PrefixExpression;
    class BinaryExpression;

    class Expression : public AST {
        PrefixExpression *m_expression;
        std::vector<BinaryExpression*> m_binaryExpressions;
    public:
        Expression(PrefixExpression*);
        ~Expression() override;

        Expression(const Expression&) = delete;
        Expression &operator=(const Expression&) = delete;
        Expression(Expression&&);
        Expression &operator=(Expression&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const PrefixExpression *const expression() const {
            return m_expression;
        }

        inline PrefixExpression *const expression() {
            return m_expression;
        }

        inline const std::vector<BinaryExpression*> &binaryExpressions() const {
            return m_binaryExpressions;
        }

        inline std::vector<BinaryExpression*> &binaryExpressions() {
            return m_binaryExpressions;
        }
    };
}

#endif
