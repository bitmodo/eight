#include <eight/ast/function.h>

#include <eight/token.h>
#include <eight/node.h>

struct token* getFunctionName(struct node* f) {
    if (HEDLEY_UNLIKELY(f->tokens == HEDLEY_NULL)) return HEDLEY_NULL;

    return getTokenAt(f->tokens, 0);
}

struct nodelist* getFunctionParameters(struct node* f) {
    if (HEDLEY_UNLIKELY(f->children == HEDLEY_NULL)) return HEDLEY_NULL;

    if (HEDLEY_UNLIKELY(getNodeAt(f->children, 0) == HEDLEY_NULL)) return HEDLEY_NULL;

    return getNodeAt(f->children, 0)->children;
}

struct node* getFunctionCodeBlock(struct node* f) {
    if (HEDLEY_UNLIKELY(f->children == HEDLEY_NULL)) return HEDLEY_NULL;

    return getNodeAt(f->children, 1);
}
