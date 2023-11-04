#ifndef CPIPP_COMMON_H
#define CPIPP_COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    ARGUMENT_INVALID = 1,
    ARGUMENT_PRINT_VERSION_INFO = 2,
    ARGUMENT_PRINT_HELP = 4,
    ARGUMENT_PRINT_LEXER = 8,
    ARGUMENT_PRINT_AST = 16,
    ARGUMENT_FILE = 32
} argument_flag_t;

#endif