#include "eight/codegen/visitor.h"

#include "../ast.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <iostream>
#include <llvm-10/llvm/IR/DerivedTypes.h>

using namespace std;
using namespace Eight;

#ifdef VISITOR_METHOD_IMPL
#  undef VISITOR_METHOD_IMPL
#endif
#define VISITOR_METHOD_IMPL(name) llvmValue Visitor::visit(const name *const node)

Visitor::Visitor(llvm::StringRef moduleID) : scope(), llvmScope(), context(), builder(context) {
    module = std::make_unique<llvm::Module>(moduleID, context);
}


llvm::Type *Visitor::toType(const Type *const type) {
    if (type->name().compare("i8") == 0) {
        return builder.getInt8Ty();
    } else if (type->name().compare("i8ptr") == 0) {
        return builder.getInt8PtrTy();
    } else {
        cerr << "Unknown type" << endl;
        return module->getTypeByName(type->name());
    }
}

VISITOR_METHOD_IMPL(AST) {
    return node->accept(this);
}

VISITOR_METHOD_IMPL(Type) {
    return toType(node);
}

VISITOR_METHOD_IMPL(Parameter) {
    return nullptr;
}

VISITOR_METHOD_IMPL(Prototype) {
    if (llvmScope.contains(node->name()))
        return llvmScope.get(node->name());

    std::vector<llvm::Type*> params;
    for (const auto &param : node->parameters()) {
        params.push_back(std::get<llvm::Type*>(visit(param.type())));
    }

    llvm::FunctionType *ft = llvm::FunctionType::get(builder.getVoidTy(), params, node->variadic());

    llvm::Function *function = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, node->name(), module.get());

    unsigned index = 0;
    for (auto &arg : function->args())
        arg.setName(node->parameters()[index++].name());

    llvmScope.set(node->name(), function);
    scope.set(node->name(), node);

    return function;
}

VISITOR_METHOD_IMPL(CodeBlock) {
    llvmScope.pushFrame();
    scope.pushFrame();

    for (const auto &statement : node->statements()) {
        visit(statement);
    }

    scope.popFrame();
    llvmScope.popFrame();

    return nullptr;
}

VISITOR_METHOD_IMPL(Statement) {
    return node->accept(this);
}

VISITOR_METHOD_IMPL(ExpressionStatement) {
    return visit(node->expression());
}

VISITOR_METHOD_IMPL(DeclarationStatement) {
    return visit(node->declaration());
}

VISITOR_METHOD_IMPL(ControlTransfer) {
    return node->accept(this);
}

VISITOR_METHOD_IMPL(Return) {
    return node->expression() ? builder.CreateRet(std::get<llvm::Value*>(visit(node->expression()))) : builder.CreateRetVoid();
}

VISITOR_METHOD_IMPL(Expression) {
    llvm::Value *val = std::get<llvm::Value*>(visit(node->expression()));

    auto es = node->binaryExpressions();
    for (auto i = es.rbegin(), e = es.rend(); i != e; i++) {
        llvm::Value *tmp = std::get<llvm::Value*>(visit(*i));
        builder.CreateStore(tmp, val);
        // val = tmp;
    }

    return val;
}

VISITOR_METHOD_IMPL(BinaryExpression) {
    return node->accept(this);
}

VISITOR_METHOD_IMPL(Assignment) {
    return visit(node->expression());
}

VISITOR_METHOD_IMPL(PrefixExpression) {
    return visit(node->expression());
}

VISITOR_METHOD_IMPL(PrimaryExpression) {
    return node->accept(this);
}

VISITOR_METHOD_IMPL(Literal) {
    return node->accept(this);
}

VISITOR_METHOD_IMPL(String) {
    std::string name;
    if (node->hasName())
        name = node->name();
    else
        name = ".str";

    return builder.CreateGlobalStringPtr(node->content(), name);
}

VISITOR_METHOD_IMPL(Identifier) {
    if (llvmScope.contains(node->name()))
        return llvmScope.get(node->name());

    llvm::Value *v;
    if ((v = module->getFunction(node->name())) != nullptr)
        return v;

    if ((v = module->getNamedValue(node->name())) != nullptr) {
        cout << "Found named" << endl;
        return v;
    }

    cerr << "Unknown identifier" << endl;
    return nullptr;
}

VISITOR_METHOD_IMPL(PostfixExpression) {
    return node->accept(this);
}

VISITOR_METHOD_IMPL(PrimaryPostfixExpression) {
    return visit(node->expression());
}

VISITOR_METHOD_IMPL(Call) {
    llvm::Function *function = (llvm::Function*) std::get<llvm::Value*>(visit(node->func()));
    if (!function)
        return nullptr;
    
    if (function->arg_size() != node->arguments().size()) {
        cerr << "Incorrect argument size" << endl;
        return nullptr;
    }

    std::vector<llvm::Value*> args;
    for (unsigned i = 0, e = node->arguments().size(); i < e; ++i)
        args.push_back(std::get<llvm::Value*>(visit(node->arguments()[i])));
    
    return builder.CreateCall(function, args, node->name());
}

VISITOR_METHOD_IMPL(ExplicitMember) {
    // AST *parent = scope.get(node->n)
    // if (!scope.contains(node->))
    // llvm::Value *parent = std::get<llvm::Value*>(visit(node->parent()));
    // if (!parent)
    //     return nullptr;
    
    // llvm::Type *parentType = parent->getType();
    // if (!parentType) {
    //     cerr << "Parent does not have a type" << endl;
    //     return nullptr;
    // }
    // if(scope.contains(parentType->is))
    // AST *p = scope.g
    // if (!parent->hasName()) {
    //     cerr << "Parent does not have name" << endl;
    //     return nullptr;
    // }

    return nullptr;
}

VISITOR_METHOD_IMPL(Declaration) {
    return node->accept(this);
}

VISITOR_METHOD_IMPL(Variable) {
    if (llvmScope.contains(node->name()))
        return llvmScope.get(node->name());

    llvm::Value *variable = nullptr;
    if (node->mutate()) {
        llvm::Function *func = builder.GetInsertBlock()->getParent();
        llvm::IRBuilder<> tmp(&func->getEntryBlock(), func->getEntryBlock().begin());
        variable = tmp.CreateAlloca(std::get<llvm::Type*>(visit(node->type())), 0, node->name());

        if (node->expression())
            builder.CreateStore(std::get<llvm::Value*>(visit(node->expression())), variable);
    } else if (node->expression()) {
        variable = std::get<llvm::Value*>(visit(node->expression()));
        // variable->setName(node->name());
    }

    if (variable) {
        llvmScope.set(node->name(), variable);
        scope.set(node->name(), node);
    }

    return variable;
}

VISITOR_METHOD_IMPL(Function) {
    llvm::Function *function = module->getFunction(node->prototype()->name());

    if (!function)
        function = (llvm::Function*) std::get<llvm::Value*>(visit(node->prototype()));
    
    if (!function) {
        return nullptr;
    }
    
    if (!function->empty()) {
        cerr << "Function \"" << node->prototype()->name() << "\" cannot be redefined" << endl;
        return nullptr;
    }

    if (!node->codeBlock())
        return function;

    llvm::IRBuilderBase::InsertPoint ip = builder.saveAndClearIP();

    llvm::BasicBlock *block = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(block);

    visit(node->codeBlock());

    builder.CreateRetVoid();
    llvm::verifyFunction(*function);

    builder.restoreIP(ip);

    return function;
}

VISITOR_METHOD_IMPL(Struct) {
    if (llvmScope.contains(node->name()))
        return llvmScope.get(node->name());
    
    llvm::Type *t = module->getTypeByName(node->name());
    if (t)
        return t;
    
    std::vector<llvm::Type*> elements;
    for (const auto &member : node->body())
        if (member->isVariable())
            elements.push_back(std::get<llvm::Type*>(visit(((Variable*) member)->type())));
    
    t = llvm::StructType::create(context, elements, node->name());
    llvmScope.set(node->name(), t);
    scope.set(node->name(), node);

    return t;
}
