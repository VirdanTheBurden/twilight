#ifndef CPIPP_SCANNER_H
#define CPIPP_SCANNER_H

#include "token.h"

typedef struct {
    const char *start;
    const char *current;
    int line;
} Scanner;

void initScanner(const char *source);
Token scanToken();

#endif