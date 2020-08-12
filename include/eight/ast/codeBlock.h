#ifndef EIGHT_AST_CODEBLOCK_H
#define EIGHT_AST_CODEBLOCK_H

#include "eight/api.h"
#include "eight/ast.h"

#include <vector>

namespace Eight {
    class Statement;

    class CodeBlock : public AST {
        std::vector<Statement*> m_statements;
    public:
        CodeBlock() = default;
        ~CodeBlock() override;

        CodeBlock(const CodeBlock&) = delete;
        CodeBlock &operator=(const CodeBlock&) = delete;
        CodeBlock(CodeBlock&&);
        CodeBlock &operator=(CodeBlock&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const std::vector<Statement*>& statements() const {
            return m_statements;
        }

        inline std::vector<Statement*>& statements() {
            return m_statements;
        }
    };
}

#endif
