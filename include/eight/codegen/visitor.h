#ifndef EIGHT_CODEGEN_VISITOR_H
#define EIGHT_CODEGEN_VISITOR_H

#include "eight/api.h"
#include "eight/ast/scope.h"

#include "../_ast_classes.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include <memory>
#include <variant>

#ifdef VISITOR_METHOD
#  undef VISITOR_METHOD
#endif
#define VISITOR_METHOD(name) llvmValue visit(const name *const)

namespace Eight {
    #ifndef llvmValue
    using llvmValue = std::variant<llvm::Value*, llvm::Type*, std::nullptr_t>;
    #endif

    struct Visitor {
        Scope<const AST*> scope;
        Scope<llvmValue> llvmScope;
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;

        llvm::Type *toType(const Type *const);

    public:
        Visitor(llvm::StringRef);

        Visitor() = delete;
        Visitor(const Visitor&) = delete;
        Visitor &operator=(const Visitor&) = delete;
        Visitor(Visitor&&) = delete;
        Visitor &operator=(Visitor&&) = delete;

        VISITOR_METHOD(AST);
        
        VISITOR_METHOD(Type);

        VISITOR_METHOD(Parameter);
        VISITOR_METHOD(Prototype);
        VISITOR_METHOD(CodeBlock);

        VISITOR_METHOD(Statement);
        VISITOR_METHOD(ExpressionStatement);
        VISITOR_METHOD(DeclarationStatement);
        
        VISITOR_METHOD(ControlTransfer);
        VISITOR_METHOD(Return);

        VISITOR_METHOD(Expression);
        
        VISITOR_METHOD(BinaryExpression);
        VISITOR_METHOD(Assignment);
        
        VISITOR_METHOD(PrefixExpression);

        VISITOR_METHOD(PrimaryExpression);

        VISITOR_METHOD(Literal);
        VISITOR_METHOD(String);

        VISITOR_METHOD(Identifier);

        VISITOR_METHOD(PostfixExpression);
        VISITOR_METHOD(PrimaryPostfixExpression);
        VISITOR_METHOD(Call);
        VISITOR_METHOD(ExplicitMember);
        
        VISITOR_METHOD(Declaration);
        VISITOR_METHOD(Variable);
        VISITOR_METHOD(Function);
        VISITOR_METHOD(Struct);
    };
}

#endif
