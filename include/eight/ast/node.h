#ifndef EIGHT_AST_NODE_H
#define EIGHT_AST_NODE_H

#include <eight/api.h>

#include <stddef.h>

HEDLEY_BEGIN_C_DECLS

typedef struct node {

} node_t;

typedef struct type {
    char* name;
} type_t;

typedef struct array {
    size_t count;
    void** value;
} array_t;

EIGHT_API
void freeType(type_t*);

HEDLEY_END_C_DECLS

#endif
