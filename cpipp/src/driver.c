#include "driver.h"
#include "lexer/lexer.h"
#include "common.h"


static char *readFile(const char *path) {
    FILE *f = fopen(path, "r");

    if (!f) {
        fprintf(stderr, "Error: file \"%s\" could not be found.", path);
        exit(74);
    }

    fseek(f, 0L, SEEK_END);
    size_t fileSize = ftell(f);
    rewind(f);

    char *buffer = (char *)malloc(fileSize + 1);

    if (!buffer) {
        fclose(f);
        fprintf(stderr, "Error: buffer could not be allocated for file \"%s\".", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, f);
    buffer[bytesRead] = '\0';

    return buffer;
}


void beginCompilation(const char *path) {
    // get source code
    char *source = readFile(path);

    // produce token stream
    lexFile(source);

    // organize into ast

    // convert to LLVM
}