#include <eight/ast/expression.h>

#include <stdlib.h>
#include <string.h>

expression_t* allocExpression(expressiontype_t type) {
    expression_t* e = malloc(sizeof(expression_t));
    if (HEDLEY_UNLIKELY(e == HEDLEY_NULL)) {
        return HEDLEY_NULL;
    }

    memset(e, 0, sizeof(expression_t));
    e->type = type;

    switch (type) {
        case ExpLiteral:
            e->data.literal = HEDLEY_NULL;
            break;
        case ExpCall:
            e->data.call = malloc(sizeof(calldata_t));
            if (HEDLEY_UNLIKELY(e->data.call == HEDLEY_NULL)) {
                free(e);
                return HEDLEY_NULL;
            }

            memset(e->data.call, 0, sizeof(calldata_t));
            break;
        case ExpSymbol:
            e->data.symbol = HEDLEY_NULL;
            break;
    }

    return e;
}

expression_t* allocLiteral(literaltype_t type) {
    expression_t* e = allocExpression(ExpLiteral);
    if (HEDLEY_UNLIKELY(e == HEDLEY_NULL)) {
        return HEDLEY_NULL;
    }

    literaldata_t* d = malloc(sizeof(literaldata_t));
    if (HEDLEY_UNLIKELY(d == HEDLEY_NULL)) {
        free(e);
        return HEDLEY_NULL;
    }

    memset(d, 0, sizeof(literaldata_t));
    d->type = type;

    switch (type) {
        case LitString:
            d->data.string = HEDLEY_NULL;
            break;
    }

    e->data.literal = d;
    return e;
}

void freeLiteral(literaldata_t* l) {
    if (HEDLEY_UNLIKELY(l == HEDLEY_NULL)) return;

    switch (l->type) {
        case LitString:
        {
            char* str = l->data.string;
            if (HEDLEY_LIKELY(str != HEDLEY_NULL)) {
                free(str);
            }

            break;
        }
    }

    free(l);
}

void freeExpression(expression_t* e) {
    if (HEDLEY_UNLIKELY(e == HEDLEY_NULL)) return;

    switch (e->type) {
        case ExpLiteral:
        {
            literaldata_t* literal = e->data.literal;
            if (HEDLEY_LIKELY(literal != HEDLEY_NULL)) {
                freeLiteral(literal);
            }

            break;
        }
        case ExpCall:
        {
            calldata_t* call = e->data.call;
            if (HEDLEY_LIKELY(call != HEDLEY_NULL)) {
                expression_t* head = call->head;
                if (HEDLEY_LIKELY(head != HEDLEY_NULL)) {
                    freeExpression(head);
                }

                if (HEDLEY_LIKELY(call->argv != HEDLEY_NULL)) {
                    for (size_t i = 0; i < call->argc; i++) {
                        freeExpression(call->argv[i]);
                    }
                    
                    free(call->argv);
                }

                free(call);
            }

            break;
        }
        case ExpSymbol:
        {
            char* symbol = e->data.symbol;
            if (HEDLEY_LIKELY(symbol != HEDLEY_NULL)) {
                free(symbol);
            }

            break;
        }
    }

    free(e);
}
