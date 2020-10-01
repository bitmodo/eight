#ifndef EIGHT_AST_STATEMENT_H
#define EIGHT_AST_STATEMENT_H

#include <eight/api.h>
#include <eight/ast/node.h>

HEDLEY_BEGIN_C_DECLS

typedef enum statementtype {
    StmtExpression,
} statementtype_t;

struct expression;

typedef union statementdata {
    struct expression* exp;
} statementdata_t;

typedef struct statement {
    node_t base;
    statementtype_t type;
    statementdata_t data;
} statement_t;

EIGHT_API
statement_t* allocStatement(statementtype_t);

EIGHT_API
void freeStatement(statement_t*);

HEDLEY_END_C_DECLS

#endif
