#include "common.h"
#include "driver.h"

const char *help = "Usage: cpipp [OPTIONS]... [FILES]...\n"
                   "A compiler for Pipp, written in C.\n\n"
                   "  --debug                    Combines --print-lexer and --print-parser.\n"
                   "  -h, --help                 Display this information.\n"
                   "  --print-lexer              Prints a .pipp file's token stream.\n"
                   "  --print-ast                Prints the AST of a pipp file.\n"
                   "  -v, --version              Display compiler version information.";

typedef enum ArgumentFlags {
    ARGUMENT_PRINT_LEXER = 1,
    ARGUMENT_PRINT_AST = 2
} argument_flag_t;


int main(int argc, const char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "cpipp: no input files provided.\n");
        exit(64);
    }
  
    if (argc == 2) {
        beginCompilation(argv[1]);
    }

    else {
        // flag_t flags = 0;

        for (int i = 0; i < argc; i++) {

        }

        fprintf(stderr, "cpipp does not support additional args yet :D");
        exit(64);
    }
}
