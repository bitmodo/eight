#include <eight/node.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <eight/token.h>

struct node* createNode(nodegroup_t group, nodetype_t type, struct tokenlist* tokens, nodelist_t* children) {
    node_t* n = malloc(sizeof(node_t));
    if (HEDLEY_UNLIKELY(n == HEDLEY_NULL)) return HEDLEY_NULL;

    memset(n, 0, sizeof(node_t));
    n->group = group;
    n->type = type;

    n->tokens = tokens;
    n->children = children;

    return n;
}

struct nodelist* createNodeList(size_t count, ...) {
    nodelist_t* list = malloc(sizeof(nodelist_t));
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) return HEDLEY_NULL;

    memset(list, 0, sizeof(nodelist_t));
    nodelist_t* end = list;

    va_list args;
    va_start(args, count);

    for (unsigned int i = 0; i < count; i++) {
        if (HEDLEY_LIKELY(i != 0)) {
            end->next = malloc(sizeof(nodelist_t));
            if (HEDLEY_UNLIKELY(end->next == HEDLEY_NULL)) {
                freeNodeList(&list);
                return HEDLEY_NULL;
            }

            end = end->next;
            memset(end, 0, sizeof(nodelist_t));
        }

        end->value = va_arg(args, node_t*);
    }

    va_end(args);

    return list;
}

void freeNode(node_t** n) {
    if (HEDLEY_LIKELY((*n)->tokens)) {
        freeTokenList(&(*n)->tokens);
    }

    if (HEDLEY_LIKELY((*n)->children)) {
        freeNodeList(&(*n)->children);
    }

    free(*n);
    *n = HEDLEY_NULL;
}

void freeNodeList(nodelist_t** l) {
    if (HEDLEY_LIKELY((*l)->next)) {
        freeNodeList(&(*l)->next);
    }
    
    if (HEDLEY_LIKELY((*l)->value)) {
        freeNode(&(*l)->value);
    }

    free(*l);
    *l = HEDLEY_NULL;
}
