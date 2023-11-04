#include "parser/parser.h"
#include "common.h"

const char *errorTypeArray[] = {"Syntax"};
ParserState parser;

void initParser() {
    parser.hadError = false;
    parser.panicMode = false;
}

static void errorAt(Token *token, ErrorType error, char *message) {

    if (parser.panicMode) return;

    fprintf(stderr, "%sError on line %d at", errorTypeArray[error], token->line);

    if (token->tokenType == TOKEN_EOF) {
        fprintf(stderr, " end of file");
    }

    else if (token->tokenType != TOKEN_ERROR) {
        fprintf(stderr, " '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
}


static void error() {

}