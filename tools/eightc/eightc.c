#include <stdio.h>
#include <stdlib.h>

#include <eight/eight.h>

int main(int argc, char* argv[]) {
    fprintf(stdout, "Eight v%s\n", getEightVersionString());

    if (argc < 2) {
        fprintf(stderr, "You must specify the file name\n");
        return EXIT_FAILURE;
    }

    if (argc > 2) {
        fprintf(stderr, "Compiling multiple files is not supported at this time\n");
        return EXIT_FAILURE;
    }

    char* filename = argv[1];
    fprintf(stdout, "Compiling file: %s\n", filename);

    compilation_t c = {"Test"};
    if (compileEightFile(filename, c)) {
        fprintf(stderr, "Unable to compile file\n");
    }

    return EXIT_SUCCESS;
}
