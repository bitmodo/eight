#ifndef EIGHT_AST_FUNCTION_H
#define EIGHT_AST_FUNCTION_H

#include "eight/ast/declaration.h"

#include <string>
#include <vector>

namespace Eight {
    class Prototype;
    class CodeBlock;

    class Function : public Declaration {
        Prototype *m_prototype;
        CodeBlock *m_codeBlock;
    public:
        Function(Prototype*, CodeBlock* = nullptr);
        virtual ~Function() override;

        Function(const Function&) = delete;
        Function &operator=(const Function&) = delete;
        Function(Function&&);
        Function &operator=(Function&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        inline const Prototype *const prototype() const {
            return m_prototype;
        }

        inline const CodeBlock *codeBlock() const {
            return m_codeBlock;
        }
    };
}

#endif
