#ifndef EIGHT_AST_STATEMENT_CONTROLTRANSFER_RETURN_H
#define EIGHT_AST_STATEMENT_CONTROLTRANSFER_RETURN_H

#include "eight/ast/statement/controlTransfer.h"

namespace Eight {
    class Expression;

    class Return : public ControlTransfer {
        Expression *m_expression;
    public:
        Return(Expression* = nullptr);
        ~Return() override;

        Return(const Return&) = delete;
        Return &operator=(const Return&) = delete;
        Return(Return&&);
        Return &operator=(Return&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const Expression *const expression() const {
            return m_expression;
        }

        inline Expression *expression() {
            return m_expression;
        }
    };
}

#endif
