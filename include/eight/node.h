#ifndef EIGHT_NODE_H
#define EIGHT_NODE_H

#include <eight/api.h>

#include <stddef.h>

HEDLEY_BEGIN_C_DECLS

enum nodegroup {
    NNone = (1 << 0),
    NDeclaration = (1 << 1),
    NStatement = (1 << 2),
} HEDLEY_FLAGS;

typedef enum nodegroup nodegroup_t;

typedef enum nodetype {
    NRoot = -1,

    NCodeBlock,

    NFunction,
    NParameter,
    NVariadic,
} nodetype_t;

struct tokenlist;

typedef struct nodelist nodelist_t;
typedef struct node node_t;

struct nodelist {
    struct node* value;
    struct nodelist* next;
};

struct node {
    enum nodegroup group;
    enum nodetype type;
    struct tokenlist* tokens;
    struct nodelist* children;
};

EIGHT_API
struct node* createNode(nodegroup_t, nodetype_t, struct tokenlist*, nodelist_t*);

EIGHT_API
struct nodelist* createNodeList(size_t, ...);

EIGHT_API
void freeNode(node_t**);

EIGHT_API
void freeNodeList(nodelist_t**);

HEDLEY_END_C_DECLS

#endif
