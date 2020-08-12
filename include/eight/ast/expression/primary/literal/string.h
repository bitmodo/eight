#ifndef EIGHT_AST_EXPRESSION_PRIMARY_LITERAL_STRING_H
#define EIGHT_AST_EXPRESSION_PRIMARY_LITERAL_STRING_H

#include "eight/ast/expression/primary/literal.h"

#include <string>

namespace Eight {
    class String : public Literal {
        const std::string m_content;
    public:
        String(const std::string&);
        ~String() override = default;

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const std::string &content() const {
            return m_content;
        }
    };
}

#endif
