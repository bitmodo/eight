#ifndef EIGHT_AST_TYPE_H
#define EIGHT_AST_TYPE_H

#include "eight/api.h"
#include "eight/ast.h"

#include <string>

namespace Eight {
    class Type : public AST {
        const std::string m_name;
    public:
        Type(const std::string&);
        ~Type() override = default;

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const std::string &name() const {
            return m_name;
        }
    };
}

#endif
