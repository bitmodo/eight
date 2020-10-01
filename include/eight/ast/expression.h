#ifndef EIGHT_AST_EXPRESSION_H
#define EIGHT_AST_EXPRESSION_H

#include <eight/api.h>
#include <eight/ast/node.h>

#include <stddef.h>

HEDLEY_BEGIN_C_DECLS

typedef enum expressiontype {
    ExpLiteral,
    ExpCall,
    ExpSymbol,
} expressiontype_t;

struct literaldata;
struct calldata;

typedef union expressiondata {
    struct literaldata* literal;
    struct calldata* call;
    char* symbol;
} expressiondata_t;

typedef struct expression {
    node_t base;
    expressiontype_t type;
    expressiondata_t data;
} expression_t;

typedef enum literaltype {
    LitString,
} literaltype_t;

typedef union literalinternaldata {
    char* string;
} literalinternaldata_t;

typedef struct literaldata {
    literaltype_t type;
    literalinternaldata_t data;
} literaldata_t;

typedef struct calldata {
    expression_t* head;
    size_t argc;
    expression_t** argv;
} calldata_t;

EIGHT_API
expression_t* allocExpression(expressiontype_t);

EIGHT_API
expression_t* allocLiteral(literaltype_t);

EIGHT_API
void freeLiteral(literaldata_t*);

EIGHT_API
void freeExpression(expression_t*);

HEDLEY_END_C_DECLS

#endif
