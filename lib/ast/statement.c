#include <eight/ast/statement.h>

#include <stdlib.h>
#include <string.h>

#include <eight/ast/expression.h>

statement_t* allocStatement(statementtype_t type) {
    statement_t* s = malloc(sizeof(statement_t));
    if (HEDLEY_UNLIKELY(s == HEDLEY_NULL)) {
        return HEDLEY_NULL;
    }

    memset(s, 0, sizeof(statement_t));
    s->type = type;

    switch (type) {
        case StmtExpression:
            s->data.exp = HEDLEY_NULL;
            break;
    }

    return s;
}

void freeStatement(statement_t* s) {
    if (HEDLEY_UNLIKELY(s == HEDLEY_NULL)) return;

    switch (s->type) {
        case StmtExpression:
            freeExpression(s->data.exp);
            break;
    }

    free(s);
}
