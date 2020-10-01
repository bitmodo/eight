#include <eight/ast/node.h>

#include <stdlib.h>

void freeType(type_t* t) {
    if (HEDLEY_LIKELY(t->name != HEDLEY_NULL)) {
        free(t->name);
    }

    free(t);
}
