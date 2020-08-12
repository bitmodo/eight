#ifndef EIGHT_AST_DECLARATION_H
#define EIGHT_AST_DECLARATION_H

#include "eight/api.h"
#include "eight/ast.h"

namespace Eight {
    class Declaration : public AST {
    public:
        Declaration() = default;
        virtual ~Declaration() override = default;

        Declaration(const std::string&);

        virtual void analyze(Sema*) override = 0;
        virtual llvmValue accept(Visitor*) const override = 0;

        virtual bool isVariable() const {
            return false;
        }
    };
}

#endif
