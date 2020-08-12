#ifndef EIGHT_AST_EXPRESSION_POSTFIX_CALL_H
#define EIGHT_AST_EXPRESSION_POSTFIX_CALL_H

#include "eight/ast/expression/postfix.h"

#include <vector>

namespace Eight {
    class Expression;

    class Call : public PostfixExpression {
        PostfixExpression *m_func;
        std::vector<Expression*> m_arguments;
    public:
        Call() = default;
        ~Call() override;

        Call(const Call&) = delete;
        Call &operator=(const Call&) = delete;
        Call(Call&&);
        Call &operator=(Call&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        virtual void setHead(PostfixExpression*) override;

        inline const PostfixExpression *const func() const {
            return m_func;
        }

        inline const std::vector<Expression*> &arguments() const {
            return m_arguments;
        }

        inline std::vector<Expression*> &arguments() {
            return m_arguments;
        }
    };
}

#endif
