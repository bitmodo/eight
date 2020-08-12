#ifndef EIGHT_AST_STATEMENT_CONTROLTRANSFER_H
#define EIGHT_AST_STATEMENT_CONTROLTRANSFER_H

#include "eight/ast/statement.h"

namespace Eight {
    class ControlTransfer : public Statement {
    public:
        virtual ~ControlTransfer() override = default;

        virtual void analyze(Sema*) override = 0;
        virtual llvmValue accept(Visitor*) const override = 0;
    };
}

#endif
