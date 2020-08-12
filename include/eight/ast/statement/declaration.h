#ifndef EIGHT_AST_STATEMENT_DECLARATION_H
#define EIGHT_AST_STATEMENT_DECLARATION_H

#include "eight/ast/statement.h"

namespace Eight {
    class Declaration;

    class DeclarationStatement : public Statement {
        Declaration *m_declaration;
    public:
        DeclarationStatement(Declaration*);
        ~DeclarationStatement() override;

        DeclarationStatement(const DeclarationStatement&) = delete;
        DeclarationStatement &operator=(const DeclarationStatement&) = delete;
        DeclarationStatement(DeclarationStatement&&);
        DeclarationStatement &operator=(DeclarationStatement&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const Declaration *const declaration() const {
            return m_declaration;
        }
    };
}

#endif
