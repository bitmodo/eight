#ifndef EIGHT_AST_AST_H
#define EIGHT_AST_AST_H

#include <string>
#include <variant>

namespace llvm {
    class Value;
    class Type;
}

namespace Eight {
    struct Sema;
    struct Visitor;

    #ifndef llvmValue
    using llvmValue = std::variant<llvm::Value*, llvm::Type*, std::nullptr_t>;
    #endif

    class AST {
    protected:
        std::string m_name = "";
    public:
        AST() = default;
        virtual ~AST() = default;

        AST(const std::string&);

        virtual void analyze(Sema*) = 0;
        virtual llvmValue accept(Visitor*) const = 0;

        inline const std::string &name() const {
            return m_name;
        }

        inline std::string &name() {
            return m_name;
        }

        inline bool hasName() const {
            return !m_name.empty();
        }
    };
}

#endif
