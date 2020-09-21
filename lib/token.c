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
    tokenlist_t* list = newTokenList();
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) return HEDLEY_NULL;

    va_list args;
    va_start(args, count);

    for (size_t i = 0; i < count; i++) {
        if (HEDLEY_UNLIKELY(addTokenToEnd(list, va_arg(args, token_t*)) == false)) {
            freeTokenList(&list);
            return HEDLEY_NULL;
        }
    }

    va_end(args);

    return list;
}

struct tokenlist* newTokenList() {
    tokenlist_t* list = malloc(sizeof(tokenlist_t));
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) return HEDLEY_NULL;

    memset(list, 0, sizeof(tokenlist_t));
    return list;
}

bool addTokenToEnd(struct tokenlist* list, struct token* t) {
    tokenlistelement_t* e = malloc(sizeof(tokenlistelement_t));
    if (HEDLEY_UNLIKELY(e == HEDLEY_NULL)) return false;

    memset(e, 0, sizeof(tokenlistelement_t));
    e->value = t;

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
struct token* getTokenAtInternal(tokenlistelement_t* e, size_t current, size_t index) {
    if (HEDLEY_UNLIKELY(e == HEDLEY_NULL)) return HEDLEY_NULL;

    if (current == index) return e->value;

    return getTokenAtInternal(e->next, current+1, index);
}

struct token* getTokenAt(struct tokenlist* l, size_t index) {
    return getTokenAtInternal(l->first, 0, index);
}

void freeToken(token_t** t) {
    if (HEDLEY_LIKELY((*t)->text)) {
        free((*t)->text);
    }

    free(*t);
    *t = HEDLEY_NULL;
}

void freeTokenListElement(tokenlistelement_t* e) {
    if (HEDLEY_LIKELY(e->value != HEDLEY_NULL)) {
        freeToken(&e->value);
    }

    if (HEDLEY_LIKELY(e->next != HEDLEY_NULL)) {
        freeTokenListElement(e->next);
    }

    free(e);
}

void freeTokenList(tokenlist_t** l) {
    if (HEDLEY_LIKELY((*l)->first != HEDLEY_NULL)) {
        freeTokenListElement((*l)->first);
    }

    free(*l);
    *l = HEDLEY_NULL;
}
