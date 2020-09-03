#include <eight/eight.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <eight/node.h>
#include <eight/parser.h>

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

int compileEight(const char* file, struct compilation c) {
    if (HEDLEY_UNLIKELY(!file)) {
        return -1;
    }

    fprintf(stdout, "Output file: %s\n", c.outfile);
    fprintf(stdout, "File contents:\n%s\n", file);

    parser_t* p = newParser();
    if (HEDLEY_UNLIKELY(!p)) {
        return -1;
    }

    prepareParser(p, file);

    // TODO(BSFishy): make sure this parsing logic is correct
    struct node* root = parseRoot(p);
    freeParser(&p);

    if (HEDLEY_UNLIKELY(!root)) {
        return -1;
    }

    if (HEDLEY_UNLIKELY(!root->children)) {
        freeNode(&root);
        return -1;
    }

    freeNode(&root);

    // TODO(BSFishy): do codegen

    return 0;
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

    // Go to the end of the file, and close the file handle if it fails
    if (HEDLEY_UNLIKELY(fseek(file, 0, SEEK_END))) {
        fclose(file);
        return -1;
    }

    // Get the current file position, and close the file handle if it fails
    long fileSize = ftell(file);
    if (HEDLEY_UNLIKELY(fileSize == -1L)) {
        fclose(file);
        return -1;
    }

    // Go to the beginning of the file, and close the file handle if it fails
    if (HEDLEY_UNLIKELY(fseek(file, 0, SEEK_SET))) {
        fclose(file);
        return -1;
    }

    // Allocate a buffer for the file, and close the file handle if it fails
    char* contents = malloc(fileSize + 1);
    if (HEDLEY_UNLIKELY(!contents)) {
        fclose(file);
        return -1;
    }

    // Read the entire file into the buffer, and close the file handle if it fails
    if (HEDLEY_UNLIKELY(fread(contents, 1, fileSize, file) != fileSize)) {
        fclose(file);
        return -1;
    }

    // Close the file handle because we don't need it anymore
    fclose(file);

    // Set the last element of the buffer to 0 indicate the end of the string
    contents[fileSize] = 0;

    // Call the compilation function with the file contents
    int value = compileEight(contents, c);

    // Free the previously allocated contents string
    free(contents);

    // Return the return value of the compilation function
    return value;
}
