#include "lexer/lexer.h"
#include "common.h"
#include <stdio.h>

extern argument_flag_t argumentFlags;

static void printToken(Token token) {
    static int line = -1;

    if (line != token.line) {
        printf("%4d ", token.line);
        line = token.line;
    }

    else {
        printf("   | ");
    }
        
    printf("%2d '%.*s'\n", token.tokenType, token.length, token.start);
}


void lexFile(const char *source) {
    Token token;

    initScanner(source);

    for (;;) {
        token = scanToken();

        if (argumentFlags & ARGUMENT_PRINT_LEXER) printToken(token);

        if (token.tokenType == TOKEN_EOF) break;

    }
    
}