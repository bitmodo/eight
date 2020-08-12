#ifndef EIGHT_AST_EXPRESSION_PRIMARY_IDENTIFIER_H
#define EIGHT_AST_EXPRESSION_PRIMARY_IDENTIFIER_H

#include "eight/ast/expression/primary.h"

#include <string>

namespace Eight {
    class Identifier : public PrimaryExpression {
        const std::string m_name;
    public:
        Identifier(const std::string&);
        virtual ~Identifier() override = default;

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const std::string &name() const {
            return m_name;
        }
    };
}

#endif
