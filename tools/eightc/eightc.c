#include <stdio.h>
#include <stdlib.h>

#include <eight/eight.h>

#include "argtable3.h"

#ifndef EIGHT_MAX_ARG_ERRORS
#  define EIGHT_MAX_ARG_ERRORS 20
#endif

#ifndef EIGHT_MAX_INPUT_FILES
#  define EIGHT_MAX_INPUT_FILES 100
#endif

struct arg_lit *help, *version;
struct arg_file *outfile, *input;
struct arg_end *end;

int main(int argc, char* argv[]) {
    void* argtable[] = {
        help = arg_litn("h", "help", 0, 1, "display the help and exit"),
        version = arg_litn("v", "version", 0, 1, "display the verison info and exit"),
        outfile = arg_filen("o", "outfile", "filename", 0, 1, "the name of the file to output to"),
        input = arg_filen(NULL, NULL, "<file>", 1, EIGHT_MAX_INPUT_FILES, "the files to compile"),
        end = arg_end(EIGHT_MAX_ARG_ERRORS),
    };

    int exitcode = EXIT_SUCCESS;
    int nerrors = arg_parse(argc, argv, argtable);

    const char* filename;
    compilation_t c;

    if (help->count > 0) {
        fprintf(stdout, "Usage: %s", argv[0]);
        arg_print_syntax(stdout, argtable, "\n");
        fprintf(stdout, "Compile Eight source code.\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        goto exit;
    }

    if (version->count > 0) {
        fprintf(stdout, "Eight v%s\n", getEightVersionString());
        goto exit;
    }

    if (nerrors > 0) {
        arg_print_errors(stderr, end, argv[0]);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        exitcode = EXIT_FAILURE;
        goto exit;
    }

    if (input->count > 1) {
        fprintf(stderr, "Compiling multiple files using a single command is not supported yet.\n");
        exitcode = EXIT_FAILURE;
        goto exit;
    }

    filename = input->filename[0];
    fprintf(stdout, "Compiling file: %s\n", filename);

    c = (compilation_t) {
        .outfile = outfile->count > 0 ? outfile->filename[0] : "File",
    };

    if (compileEightFile(filename, c)) {
        fprintf(stderr, "Unable to compile file\n");
    }

exit:
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}
