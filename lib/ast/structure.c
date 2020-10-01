#include <eight/ast/structure.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <eight/ast/statement.h>

structure_t* allocStructure(structuretype_t type) {
    structure_t* s = malloc(sizeof(structure_t));
    if (HEDLEY_UNLIKELY(s == HEDLEY_NULL)) {
        return HEDLEY_NULL;
    }

    memset(s, 0, sizeof(structure_t));
    s->type = type;

    switch (type) {
        case StctFunction:
            s->data.function = malloc(sizeof(functiondata_t));
            if (HEDLEY_UNLIKELY(s->data.function == HEDLEY_NULL)) {
                free(s);
                return HEDLEY_NULL;
            }

            memset(s->data.function, 0, sizeof(functiondata_t));
            break;
        case StctStructure:
            s->data.structure = malloc(sizeof(structuredata_t));
            if (HEDLEY_UNLIKELY(s->data.structure == HEDLEY_NULL)) {
                free(s);
                return HEDLEY_NULL;
            }

            memset(s->data.structure, 0, sizeof(structuredata_t));
            break;
        case StctClass:
            s->data.cls = malloc(sizeof(classdata_t));
            if (HEDLEY_UNLIKELY(s->data.cls == HEDLEY_NULL)) {
                free(s);
                return HEDLEY_NULL;
            }

            memset(s->data.cls, 0, sizeof(classdata_t));
            break;
    }

    return s;
}

void freeStructure(structure_t* s) {
    if (HEDLEY_UNLIKELY(s == HEDLEY_NULL)) return;

    switch (s->type) {
        case StctFunction:
        {
            functiondata_t* function = s->data.function;
            if (HEDLEY_LIKELY(function != HEDLEY_NULL)) {
                functionparameters_t* params = function->params;
                if (HEDLEY_LIKELY(params != HEDLEY_NULL)) {
                    for (size_t i = 0; i < params->count; i++) {
                        parameter_t* param = params->array[i];
                        if (HEDLEY_LIKELY(param->type.name != HEDLEY_NULL)) {
                            free(param->type.name);
                        }

                        if (HEDLEY_LIKELY(param->name != HEDLEY_NULL)) {
                            free(param->name);
                        }

                        free(param);
                    }

                    free(params->array);
                    free(params);
                }

                if (HEDLEY_LIKELY(function->statements != HEDLEY_NULL)) {
                    for (size_t i = 0; i < function->statementCount; i++) {
                        freeStatement(function->statements[i]);
                    }

                    free(function->statements);
                }

                free(function);
            }

            break;
        }
        case StctStructure:
        {
            structdata_t* structure = s->data.structure;
            if (HEDLEY_LIKELY(structure != HEDLEY_NULL)) {
                free(structure);
            }

            break;
        }
        case StctClass:
        {
            classdata_t* cls = s->data.cls;
            if (HEDLEY_LIKELY(cls != HEDLEY_NULL)) {
                free(cls);
            }

            break;
        }
    }

    if (HEDLEY_LIKELY(s->name != HEDLEY_NULL)) {
        free(s->name);
    }

    free(s);
}
