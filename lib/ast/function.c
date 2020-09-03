#include <eight/ast/function.h>

#include <eight/token.h>
#include <eight/node.h>

struct token* getFunctionName(struct node* f) {
    if (HEDLEY_UNLIKELY(f->tokens == HEDLEY_NULL)) return HEDLEY_NULL;

    return f->tokens->value;
}

struct nodelist* getFunctionParameters(struct node* f) {
    if (HEDLEY_UNLIKELY(f->children == HEDLEY_NULL)) return HEDLEY_NULL;

    if (HEDLEY_UNLIKELY(f->children->value == HEDLEY_NULL)) return HEDLEY_NULL;

    return f->children->value->children;
}

struct node* getFunctionCodeBlock(struct node* f) {
    if (HEDLEY_UNLIKELY(f->children == HEDLEY_NULL)) return HEDLEY_NULL;

    if (HEDLEY_UNLIKELY(f->children->next == HEDLEY_NULL)) return HEDLEY_NULL;

    return f->children->next->value;
}
