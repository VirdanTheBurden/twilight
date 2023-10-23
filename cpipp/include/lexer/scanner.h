#ifndef CPIPP_SCANNER_H
#define CPIPP_SCANNER_H

#include "token.h"

void initScanner(const char *source);
Token scanToken();

#endif