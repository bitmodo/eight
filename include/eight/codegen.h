#ifndef EIGHT_CODEGEN_H
#define EIGHT_CODEGEN_H

#include <eight/api.h>

#include <llvm-c/Types.h>

HEDLEY_BEGIN_C_DECLS

typedef struct cgcontext {
    LLVMContextRef context;
    LLVMModuleRef module;
    LLVMBuilderRef builder;
} cgcontext_t;

EIGHT_API
cgcontext_t* allocCodegenContext(const char*);

struct array;
struct expression;
struct statement;
struct structure;

EIGHT_API
void codegen(cgcontext_t*, struct array*);

EIGHT_API
LLVMValueRef codegenExpression(cgcontext_t*, struct expression*);

EIGHT_API
LLVMValueRef codegenStatement(cgcontext_t*, struct statement*);

EIGHT_API
LLVMValueRef codegenStructure(cgcontext_t*, struct structure*);

EIGHT_API
void freeCodegenContext(cgcontext_t*);

HEDLEY_END_C_DECLS

#endif
