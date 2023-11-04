#include "common.h"
#include "driver.h"
#include "lexer/token.h"
#define __CPIPP_VERSION__ "0.1.0"

const char *help = "Usage: ctwi [OPTIONS]... [FILES]...\n"
                   "A compiler for Twilight, written in C.\n\n"
                   "  --debug                    Combines --print-lexer and --print-parser.\n"
                   "  -h, --help                 Display this information.\n"
                   "  --print-lexer              Prints a .pipp file's token stream.\n"
                   "  --print-ast                Prints the AST of a pipp file.\n"
                   "  -v, --version              Display compiler version information.\n";


argument_flag_t argumentFlags = 0;


argument_flag_t checkArgumentType(const char *argument) {

    if (strncmp(argument, "-h", 2) == 0 || strncmp(argument, "--help", 6) == 0) {
        return ARGUMENT_PRINT_HELP;
    }

    else if (strncmp(argument, "-v", 2) == 0 || strncmp(argument, "--version", 9) == 0) {
        return ARGUMENT_PRINT_VERSION_INFO;
    }

    else if (strncmp(argument, "--debug", 6) == 0) {
        return ARGUMENT_PRINT_LEXER | ARGUMENT_PRINT_AST;
    }

    else if (strncmp(argument, "--print-lexer", 13) == 0) {
        return ARGUMENT_PRINT_LEXER;
    }

    else if (strncmp(argument, "--print-ast", 11) == 0) {
        return ARGUMENT_PRINT_AST;
    }

    else if (strncmp((char *)(argument + strlen(argument) - 4), "pipp", 4) == 0) {
        return ARGUMENT_FILE;
    }
    
    return ARGUMENT_INVALID;
}


int main(int argc, const char *argv[]) {

    if (argc == 1) {
        fprintf(stderr, "ctwi: no input files provided.\n");
        exit(64);
    }
  
    else if (argc == 2) {

        argument_flag_t flag = checkArgumentType(argv[1]);

        if (flag & ARGUMENT_PRINT_HELP) {
            printf("%s", help);
            exit(0);
        }

        else if (flag & ARGUMENT_PRINT_VERSION_INFO) {
            printf("ctwi version %s\n", __CPIPP_VERSION__);
            exit(0);
        }

        else if (flag & ARGUMENT_FILE) {
            compile(argv[1]);
            exit(0);
        }

        fprintf(stderr, "Unknown modifier \"%s\".\n", argv[1]);
        exit(64);
    }

    else {

        // last argument is a assumed to be a file path, so we can stop iterating early
        for (int i = 1; i < argc - 1; i++) {
            argument_flag_t flag = checkArgumentType(argv[i]);

            if (flag & ARGUMENT_INVALID) {
                fprintf(stderr, "Unknown modifier \"%s\"\n", argv[i]);
                exit(64);
            }

            else if ((ARGUMENT_PRINT_HELP | ARGUMENT_PRINT_VERSION_INFO) & checkArgumentType(argv[1])) {
                fprintf(stderr, "Modifier \"%s\" must be used by itself.\n", argv[i]);
                exit(64);
            }

            else {
                argumentFlags |= flag;
            }    
        }

        compile(argv[argc - 1]);
        exit(0);
    }
}
