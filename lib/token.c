#include <eight/token.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

struct token* createToken(tokengroup_t group, tokentype_t type, unsigned line, unsigned column, char* text) {
    token_t* t = malloc(sizeof(token_t));
    if (HEDLEY_UNLIKELY(t == HEDLEY_NULL)) return HEDLEY_NULL;

    memset(t, 0, sizeof(token_t));

    t->group = group;
    t->type = type;

    t->line = line;
    t->column = column;

    t->text = text;

    return t;
}

struct tokenlist* createTokenList(size_t count, ...) {
    tokenlist_t* list = malloc(sizeof(tokenlist_t));
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) return HEDLEY_NULL;

    memset(list, 0, sizeof(tokenlist_t));
    tokenlist_t* end = list;

    va_list args;
    va_start(args, count);

    for (unsigned int i = 0; i < count; i++) {
        if (HEDLEY_LIKELY(i != 0)) {
            end->next = malloc(sizeof(tokenlist_t));
            if (HEDLEY_UNLIKELY(end->next == HEDLEY_NULL)) {
                freeTokenList(&list);
                return HEDLEY_NULL;
            }

            end = end->next;
            memset(end, 0, sizeof(tokenlist_t));
        }

        end->value = va_arg(args, token_t*);
    }

    va_end(args);

    return list;
}

void freeToken(token_t** t) {
    if (HEDLEY_LIKELY((*t)->text)) {
        free((*t)->text);
    }

    free(*t);
    *t = HEDLEY_NULL;
}

void freeTokenList(tokenlist_t** l) {
    if (HEDLEY_LIKELY((*l)->next)) {
        freeTokenList(&(*l)->next);
    }

    if (HEDLEY_LIKELY((*l)->value)) {
        freeToken(&(*l)->value);
    }

    free(*l);
    *l = HEDLEY_NULL;
}
