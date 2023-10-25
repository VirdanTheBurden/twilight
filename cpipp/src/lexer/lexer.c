#include "lexer/lexer.h"
#include <stdio.h>

void lexFile(const char *source) {
    Token token;
    int line = -1;

    initScanner(source);

    for (;;) {
        token = scanToken();
        if (line != token.line) {
            printf("%4d ", token.line);
            line = token.line;
        }

        else {
            printf("   | ");
        }
        
        printf("%2d '%.*s'\n", token.tokenType, token.length, token.start);
        
        if (token.tokenType == TOKEN_EOF) break;
    }
    
}