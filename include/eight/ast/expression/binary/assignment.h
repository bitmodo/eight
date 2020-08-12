#ifndef EIGHT_AST_EXPRESSION_BINARY_ASSIGNMENT_H
#define EIGHT_AST_EXPRESSION_BINARY_ASSIGNMENT_H

#include "eight/ast/expression/binary.h"

namespace Eight {
    class PrefixExpression;

    class Assignment : public BinaryExpression {
        PrefixExpression *m_expression;
    public:
        Assignment(PrefixExpression*);
        virtual ~Assignment() override;

        Assignment(const Assignment&) = delete;
        Assignment &operator=(const Assignment&) = delete;
        Assignment(Assignment&&);
        Assignment &operator=(Assignment&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const PrefixExpression *const expression() const {
            return m_expression;
        }
    };
}

#endif
