#ifndef EIGHT_AST_DECLARATION_STRUCT_H
#define EIGHT_AST_DECLARATION_STRUCT_H

#include "eight/ast/declaration.h"

#include <string>
#include <vector>

namespace Eight {
    class Struct : public Declaration {
        const std::string m_name;
        std::vector<Declaration*> m_body;
    public:
        Struct(const std::string&);
        virtual ~Struct() override;

        Struct(const Struct&) = delete;
        Struct &operator=(const Struct&) = delete;
        Struct(Struct&&);
        Struct &operator=(Struct&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const std::string &name() const {
            return m_name;
        }

        inline const std::vector<Declaration*> &body() const {
            return m_body;
        }

        inline std::vector<Declaration*> &body() {
            return m_body;
        }
    };
}

#endif
