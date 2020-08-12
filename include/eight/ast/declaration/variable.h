#ifndef EIGHT_AST_DECLARATION_VARIABLE_H
#define EIGHT_AST_DECLARATION_VARIABLE_H

#include "eight/ast/declaration.h"

#include <string>

namespace Eight {
    class Type;
    class Expression;

    class Variable : public Declaration {
        bool m_mutable;
        // std::string m_name;
        Type *m_type;
        Expression *m_expression;
    public:
        Variable(std::string, Type*, Expression*, bool = false);
        ~Variable() override;

        Variable(const Variable&) = delete;
        Variable &operator=(const Variable&) = delete;
        Variable(Variable&&);
        Variable &operator=(Variable&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        virtual bool isVariable() const override {
            return true;
        }

        inline const bool mutate() const {
            return m_mutable;
        }

        // inline const std::string &name() const {
        //     return m_name;
        // }

        inline const Type *const type() const {
            return m_type;
        }

        inline const Expression *const expression() const {
            return m_expression;
        }

        inline Expression *const expression() {
            return m_expression;
        }
    };
}

#endif
