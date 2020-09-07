#ifndef EIGHT_NODE_H
#define EIGHT_NODE_H

#include <eight/api.h>

#include <stdbool.h>
#include <stddef.h>

HEDLEY_BEGIN_C_DECLS

enum nodegroup {
    NNone = (1 << 0),

    NDeclaration = (1 << 1),
    NStatement = (1 << 2),
    NExpression = (1 << 3),

    NValue = (1 << 4),
} HEDLEY_FLAGS;

typedef enum nodegroup nodegroup_t;

typedef enum nodetype {
    NRoot = -1,

    NCodeBlock,

    NFunction,
    NParameter,
    NVariadic,

    NCall,

    NString,
} nodetype_t;

struct tokenlist;

typedef struct nodelistelement nodelistelement_t;
typedef struct nodelist nodelist_t;
typedef struct node node_t;

struct nodelistelement {
    struct nodelistelement* previous;
    struct node* value;
    struct nodelistelement* next;
};

struct nodelist {
    size_t count;
    struct nodelistelement* first;
    struct nodelistelement* last;
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
struct nodelist* newNodeList();

EIGHT_API
bool addNodeToEnd(struct nodelist*, struct node*);

EIGHT_API
struct node* getNodeAt(struct nodelist*, size_t);

EIGHT_API
void freeNode(node_t**);

EIGHT_API
void freeNodeList(nodelist_t**);

HEDLEY_END_C_DECLS

#endif
