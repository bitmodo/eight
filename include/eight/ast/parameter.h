#ifndef EIGHT_AST_PARAMETER_H
#define EIGHT_AST_PARAMETER_H

#include "eight/api.h"
#include "eight/ast.h"

#include <string>

namespace Eight {
    class Type;

    class Parameter : public AST {
        std::string m_name;
        Type *m_type;
    public:
        Parameter(const std::string&, Type*);
        ~Parameter() override;

        Parameter(const Parameter&) = delete;
        Parameter &operator=(const Parameter&) = delete;
        Parameter(Parameter&&);
        Parameter &operator=(Parameter&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const std::string &name() const {
            return m_name;
        }

        inline const Type *const type() const {
            return m_type;
        }
    };
}

#endif
