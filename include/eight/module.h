#ifndef EIGHT_MODULE_H
#define EIGHT_MODULE_H

#include <eight/api.h>

HEDLEY_BEGIN_C_DECLS

typedef struct module {
    const char* name;
} module_t;

EIGHT_API
struct module* getModule(const char*);

HEDLEY_END_C_DECLS

#endif
