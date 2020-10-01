#ifndef EIGHT_AST_STRUCTURE_H
#define EIGHT_AST_STRUCTURE_H

#include <eight/api.h>
#include <eight/ast/node.h>

#include <stdbool.h>
#include <stddef.h>

HEDLEY_BEGIN_C_DECLS

typedef enum structuretype {
    StctFunction,
    StctStructure,
    StctClass,
} structuretype_t;

typedef union structuredata {
    struct functiondata* function;
    struct structdata* structure;
    struct classdata* cls;
} structuredata_t;

typedef struct structure {
    node_t base;
    structuretype_t type;
    char* name;
    structuredata_t data;
} structure_t;

typedef struct parameter {
    type_t type;
    char* name;
} parameter_t;

typedef struct functionparameters {
    bool variadic;
    size_t count;
    parameter_t** array;
} functionparameters_t;

typedef struct functiondata {
    functionparameters_t* params;
    size_t statementCount;
    struct statement** statements;
} functiondata_t;

typedef struct structdata {

} structdata_t;

typedef struct classdata {

} classdata_t;

EIGHT_API
structure_t* allocStructure(structuretype_t);

EIGHT_API
void freeStructure(structure_t*);

HEDLEY_END_C_DECLS

#endif
