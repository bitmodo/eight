#ifndef EIGHT_AST_PROTOTYPE_H
#define EIGHT_AST_PROTOTYPE_H

#include "eight/api.h"
#include "eight/ast.h"

#include "eight/ast/parameter.h"

#include <string>
#include <vector>

namespace Eight {
    class Prototype : public AST {
        const std::string m_name;
        std::vector<Parameter> m_parameters;
        const bool m_variadic;
    public:
        Prototype(const std::string&, bool = false);
        ~Prototype() override = default;

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const std::string &name() const {
            return m_name;
        }

        inline const std::vector<Parameter> &parameters() const {
            return m_parameters;
        }

        inline std::vector<Parameter> &parameters() {
            return m_parameters;
        }

        inline const bool variadic() const {
            return m_variadic;
        }
    };
}

#endif
