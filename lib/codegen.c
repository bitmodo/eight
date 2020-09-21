#include <eight/codegen.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <llvm-c/Core.h>

#include <eight/node.h>

struct codegencontext {
    LLVMContextRef context;
    LLVMModuleRef module;
    LLVMBuilderRef builder;
};

struct codegencontext* initCodegen(const char* name) {
    codegencontext_t* ctx = malloc(sizeof(codegencontext_t));
    if (HEDLEY_UNLIKELY(ctx == HEDLEY_NULL)) return HEDLEY_NULL;

    memset(ctx, 0, sizeof(codegencontext_t));
    ctx->context = LLVMContextCreate();
    ctx->module = LLVMModuleCreateWithNameInContext(name, ctx->context);
    ctx->builder = LLVMCreateBuilderInContext(ctx->context);

    return ctx;
}

void indent(size_t indentation) {
    for (size_t i = indentation; i > 0; i--) {
        fprintf(stdout, "\t");
    }
}

void prnt(struct node* n, size_t indentation) {
    if (HEDLEY_UNLIKELY(n == HEDLEY_NULL)) return;

    indent(indentation);

    char* t;
    switch (n->type) {
        case NRoot:
            t = "root"; break;
        case NFunction:
            t = "function"; break;
        case NParameter:
            t = "parameter"; break;
        case NCodeBlock:
            t = "code block"; break;
        default:
            t = "default"; break;
    }
    fprintf(stdout, "%s\n", t);

    // fprintf(stdout, "%p %zu %p\n", n->children, n->children->count, n->children->first);

    if (HEDLEY_LIKELY(n->children != HEDLEY_NULL)) {
        for (nodelistelement_t* e = n->children->first; e != HEDLEY_NULL; e = e->next) {
            // fprintf(stdout, "here\n");
            prnt(e->value, indentation+1);
        }
    }
    // if (n->type == NNone) {
    //     fprintf()
    // }
}

LLVMValueRef* codegen(struct codegencontext* ctx, struct node* n) {
    prnt(n, 0);

    return HEDLEY_NULL;
}

void freeCodegenContext(struct codegencontext** ctx) {
    LLVMDisposeBuilder((*ctx)->builder);
    LLVMDisposeModule((*ctx)->module);
    LLVMContextDispose((*ctx)->context);

    free(*ctx);
    *ctx = HEDLEY_NULL;
}
