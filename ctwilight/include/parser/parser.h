#ifndef CPIPP_PARSER_H
#define CPIPP_PARSER_H

#include "lexer/token.h"
#include "common.h"

typedef struct ParserState {
    bool hadError;
    bool panicMode;
} ParserState;

typedef enum {
    SYNTAX_ERROR,
} ErrorType;

#endif