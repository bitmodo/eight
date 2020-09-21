#ifndef EIGHT_CODEGEN_H
#define EIGHT_CODEGEN_H

#include <eight/api.h>

#include <llvm-c/Types.h>

HEDLEY_BEGIN_C_DECLS

struct node;

typedef struct codegencontext codegencontext_t;

EIGHT_API
struct codegencontext* initCodegen(const char*);

EIGHT_API
LLVMValueRef* codegen(struct codegencontext*, struct node*);

EIGHT_API
void freeCodegenContext(struct codegencontext**);

HEDLEY_END_C_DECLS

#endif
