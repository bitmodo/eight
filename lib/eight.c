#include <eight/eight.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "lexer.h"

#include <eight/ast/expression.h>
#include <eight/ast/statement.h>
#include <eight/ast/structure.h>

#include <eight/codegen.h>

#include <llvm-c/Core.h>

unsigned getEightVersionMajor() {
    return EIGHT_VERSION_MAJOR;
}

unsigned getEightVersionMinor() {
    return EIGHT_VERSION_MINOR;
}

unsigned getEightVersionRevision() {
    return EIGHT_VERSION_REVISION;
}

unsigned getEightVersion() {
    return EIGHT_VERSION;
}

const char* getEightVersionString() {
    return HEDLEY_STRINGIFY(EIGHT_VERSION_MAJOR) "." HEDLEY_STRINGIFY(EIGHT_VERSION_MINOR) "." HEDLEY_STRINGIFY(EIGHT_VERSION_REVISION);
}

int compileEightFile(const char* filename, struct compilation c) {
    if (HEDLEY_UNLIKELY(!filename)) {
        return -1;
    }

    // Open the file in read binary mode
    FILE* file = fopen(filename, "rb");
    if (HEDLEY_UNLIKELY(!file)) {
        return -1;
    }

    // Initialize the scanner
    yyscan_t scan;
    yylex_init(&scan);
    yyset_in(file, scan);

    // Parse the file
    array_t* tree;
    do {
        // If the parser encountered an error, return
        if (HEDLEY_UNLIKELY(yyparse(scan, &tree) != 0)) {
            yylex_destroy(scan);
            fclose(file);
            return -1;
        }
    } while (HEDLEY_UNLIKELY(!feof(file)));

    // Cleanup everything from parsing
    yylex_destroy(scan);
    fclose(file);

    // fprintf(stdout, "size: %zu\n", s->count);

    // for (size_t i = 0; i < s->count; i++) {
    //     structure_t* st = s->value[i];
    //     fprintf(stdout, "Name: %s\n", st->name);
    //     switch (st->type) {
    //         case StctFunction:
    //             fprintf(stdout, "\tParameters:\n");
    //             functionparameters_t* params = st->data.function->params;
    //             for (size_t i = 0; i < params->count; i++) {
    //                 parameter_t* param = params->array[i];
    //                 fprintf(stdout, "\t\t%s: %s\n", param->name, param->type.name);
    //             }

    //             fprintf(stdout, "\t\tVariadic: %s\n", params->variadic ? "true" : "false");
    //             fprintf(stdout, "\tStatements: %zu\n", st->data.function->statementCount);

    //             break;
    //         default:
    //             fprintf(stdout, "Unimplemented function\n");
    //             break;
    //     }

    //     freeStructure(st);
    // }

    cgcontext_t* ctx = allocCodegenContext("test");
    if (HEDLEY_UNLIKELY(ctx == HEDLEY_NULL)) {
        return -1;
    }

    codegen(ctx, tree);
    LLVMDumpModule(ctx->module);

    for (size_t i = 0; i < tree->count; i++) {
        freeStructure((structure_t*) tree->value[i]);
    }

    free(tree->value);
    free(tree);

    freeCodegenContext(ctx);
    LLVMShutdown();

    // Return the return value of the compilation function
    return 0;
}
