#ifndef EIGHT_AST_STATEMENT_H
#define EIGHT_AST_STATEMENT_H

#include "eight/api.h"
#include "eight/ast.h"

namespace Eight {
    class Expression;

    class Statement : public AST {
    public:
        virtual ~Statement() override = default;

        virtual void analyze(Sema*) override = 0;
        virtual llvmValue accept(Visitor*) const override = 0;
    //     Expression *m_expression;
    // public:
    //     Statement(Expression*);
    //     virtual ~Statement() override;

    //     Statement(const Statement&) = delete;
    //     Statement &operator=(const Statement&) = delete;
    //     Statement(Statement&&);
    //     Statement &operator=(Statement&&);

    //     llvm::Value *accept(Visitor*) const override;

    //     inline const Expression *const expression() const {
    //         return m_expression;
    //     }

    //     inline Expression *expression() {
    //         return m_expression;
    //     }
    };
}

#endif
