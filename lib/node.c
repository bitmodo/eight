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
    nodelist_t* list = newNodeList();
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) return HEDLEY_NULL;

    va_list args;
    va_start(args, count);

    for (size_t i = 0; i < count; i++) {
        if (HEDLEY_UNLIKELY(addNodeToEnd(list, va_arg(args, node_t*)) == false)) {
            freeNodeList(&list);
            return HEDLEY_NULL;
        }
    }

    va_end(args);

    return list;
}

struct nodelist* newNodeList() {
    nodelist_t* list = malloc(sizeof(nodelist_t));
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) return HEDLEY_NULL;

    memset(list, 0, sizeof(nodelist_t));
    return list;
}

bool addNodeToEnd(struct nodelist* list, struct node* n) {
    nodelistelement_t* e = malloc(sizeof(nodelistelement_t));
    if (HEDLEY_UNLIKELY(e == HEDLEY_NULL)) return false;

    memset(e, 0, sizeof(nodelistelement_t));
    e->value = n;

    if (HEDLEY_UNLIKELY(list->first == HEDLEY_NULL)) {
        list->first = e;
    } else {
        e->previous = list->last;
    }

    list->last = e;
    list->count++;

    return true;
}

HEDLEY_PRIVATE
struct node* getNodeAtInternal(nodelistelement_t* e, size_t current, size_t index) {
    if (HEDLEY_UNLIKELY(e == HEDLEY_NULL)) return HEDLEY_NULL;

    if (current == index) return e->value;

    return getNodeAtInternal(e->next, current+1, index);
}

struct node* getNodeAt(struct nodelist* l, size_t index) {
    return getNodeAtInternal(l->first, 0, index);
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

void freeNodeListElement(nodelistelement_t* e) {
    if (HEDLEY_LIKELY(e->value != HEDLEY_NULL)) {
        freeNode(&e->value);
    }

    if (HEDLEY_LIKELY(e->next != HEDLEY_NULL)) {
        freeNodeListElement(e->next);
    }

    free(e);
}

void freeNodeList(nodelist_t** l) {
    if (HEDLEY_LIKELY((*l)->first != HEDLEY_NULL)) {
        freeNodeListElement((*l)->first);
    }

    free(*l);
    *l = HEDLEY_NULL;
}
