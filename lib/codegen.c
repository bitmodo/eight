#include <eight/codegen.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <eight/ast/node.h>
#include <eight/ast/expression.h>
#include <eight/ast/statement.h>
#include <eight/ast/structure.h>

#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>

cgcontext_t* allocCodegenContext(const char* name) {
    cgcontext_t* ctx = malloc(sizeof(cgcontext_t));
    if (HEDLEY_UNLIKELY(ctx == HEDLEY_NULL)) {
        return HEDLEY_NULL;
    }

    ctx->context = LLVMContextCreate();
    if (HEDLEY_UNLIKELY(ctx->context == HEDLEY_NULL)) {
        free(ctx);
        return HEDLEY_NULL;
    }

    ctx->module = LLVMModuleCreateWithNameInContext(name, ctx->context);
    if (HEDLEY_UNLIKELY(ctx->module == HEDLEY_NULL)) {
        LLVMContextDispose(ctx->context);
        free(ctx);
        return HEDLEY_NULL;
    }

    ctx->builder = LLVMCreateBuilderInContext(ctx->context);
    if (HEDLEY_UNLIKELY(ctx->builder == HEDLEY_NULL)) {
        LLVMDisposeModule(ctx->module);
        LLVMContextDispose(ctx->context);
        free(ctx);
        return HEDLEY_NULL;
    }

    return ctx;
}

void codegen(cgcontext_t* ctx, struct array* tree) {
    for (size_t i = 0; i < tree->count; i++) {
        codegenStructure(ctx, (structure_t*) tree->value[i]);
    }
}

HEDLEY_PRIVATE
static LLVMValueRef codegenLiteral(cgcontext_t* ctx, expression_t* e) {
    literaldata_t* data = e->data.literal;

    switch (data->type) {
        case LitString:
            return LLVMBuildGlobalStringPtr(ctx->builder, data->data.string, "");
        default:
            fprintf(stderr, "The literal has not been implemented\n");
            return HEDLEY_NULL;
    }
}

HEDLEY_PRIVATE
static LLVMValueRef codegenCall(cgcontext_t* ctx, expression_t* e) {
    calldata_t* data = e->data.call;

    LLVMValueRef head = codegenExpression(ctx, data->head);
    if (HEDLEY_UNLIKELY(head == HEDLEY_NULL)) {
        return HEDLEY_NULL;
    }

    LLVMValueRef* args = malloc(sizeof(LLVMValueRef) * data->argc);
    if (HEDLEY_UNLIKELY(args == HEDLEY_NULL)) {
        return HEDLEY_NULL;
    }

    for (size_t i = 0; i < data->argc; i++) {
        args[i] = codegenExpression(ctx, data->argv[i]);
    }

    LLVMValueRef call = LLVMBuildCall(ctx->builder, head, args, data->argc, "");
    free(args);

    return call;
}

LLVMValueRef codegenExpression(cgcontext_t* ctx, struct expression* e) {
    switch (e->type) {
        case ExpLiteral:
            return codegenLiteral(ctx, e);
        case ExpCall:
            return codegenCall(ctx, e);
        case ExpSymbol:
            return LLVMGetNamedFunction(ctx->module, e->data.symbol);
        default:
            fprintf(stderr, "The expression has not been implemented\n");
            return HEDLEY_NULL;
    }
}

LLVMValueRef codegenStatement(cgcontext_t* ctx, struct statement* s) {
    switch (s->type) {
        case StmtExpression:
            return codegenExpression(ctx, s->data.exp);
        default:
            fprintf(stderr, "The statement has not been implemented\n");
            return HEDLEY_NULL;
    }
}

HEDLEY_PRIVATE
static LLVMValueRef codegenPrototype(cgcontext_t* ctx, struct structure* s) {
    functiondata_t* data = s->data.function;

    LLVMTypeRef* types = malloc(sizeof(LLVMTypeRef) * data->params->count);
    for (size_t i = 0; i < data->params->count; i++) {
        const char* name = data->params->array[i]->type.name;
        if (strcmp(name, "i8ptr") == 0) {
            types[i] = LLVMPointerType(LLVMInt8TypeInContext(ctx->context), 0);
        } else {
            types[i] = LLVMGetTypeByName(ctx->module, name);
        }
    }

    LLVMTypeRef type = LLVMFunctionType(LLVMVoidTypeInContext(ctx->context), types, data->params->count, data->params->variadic);
    free(types);

    return LLVMAddFunction(ctx->module, s->name, type);
}

HEDLEY_PRIVATE
static LLVMValueRef codegenFunction(cgcontext_t* ctx, struct structure* s) {
    LLVMValueRef func = LLVMGetNamedFunction(ctx->module, s->name);
    if (HEDLEY_UNPREDICTABLE(func == HEDLEY_NULL)) {
        func = codegenPrototype(ctx, s);
    }

    if (HEDLEY_UNLIKELY(func == HEDLEY_NULL)) {
        return HEDLEY_NULL;
    }

    functiondata_t* data = s->data.function;

    if (HEDLEY_LIKELY(data->statementCount > 0)) {
        LLVMBasicBlockRef bb = LLVMAppendBasicBlockInContext(ctx->context, func, "entry");
        LLVMPositionBuilderAtEnd(ctx->builder, bb);

        bool hasReturn = false;
        for (size_t i = 0; i < data->statementCount; i++) {
            LLVMValueRef val = codegenStatement(ctx, data->statements[i]);

            if (HEDLEY_UNLIKELY(LLVMIsAReturnInst(val))) {
                hasReturn = true;
            }
        }

        if (HEDLEY_UNLIKELY(hasReturn == false)) {
            LLVMBuildRetVoid(ctx->builder);
        }
    }

    LLVMVerifyFunction(func, LLVMPrintMessageAction);

    return func;
}

LLVMValueRef codegenStructure(cgcontext_t* ctx, struct structure* s) {
    switch (s->type) {
        case StctFunction:
            return codegenFunction(ctx, s);
        default:
            fprintf(stderr, "Structures other than functions are not implemented yet\n");
            return HEDLEY_NULL;
    }

    // fprintf(stdout, "Here: %s\n", s->name);
}

void freeCodegenContext(cgcontext_t* ctx) {
    if (HEDLEY_UNLIKELY(ctx == HEDLEY_NULL)) return;

    if (HEDLEY_LIKELY(ctx->builder != HEDLEY_NULL)) {
        LLVMDisposeBuilder(ctx->builder);
    }

    if (HEDLEY_LIKELY(ctx->module != HEDLEY_NULL)) {
        LLVMDisposeModule(ctx->module);
    }

    if (HEDLEY_LIKELY(ctx->context != HEDLEY_NULL)) {
        LLVMContextDispose(ctx->context);
    }

    free(ctx);
}
