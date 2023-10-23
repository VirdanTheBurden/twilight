#include "lexer/scanner.h"
#include "lexer/token.h"
#include "common.h"

int total_keyword_search_time = 0;

typedef struct scanner_t {
    const char *start;
    const char *current;
    int line;
} Scanner;


Scanner scanner;

void initScanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}


static int currentLineLength() {
    return scanner.current - scanner.start;
}


static bool isAtEnd() {
    return *scanner.current == '\0';
}


static bool isDigit(char c) {
    return '0' <= c && c <= '9'; 
}


static bool isAlpha(char c) {
    return ('a' <= c && c <= 'z') || 
           ('A' <= c && c <= 'Z') || 
           (c == '_');
}


static Token emitToken(TokenType type) {
    Token token;
    token.tokenType = type;
    token.start = scanner.start;
    token.length = (int)(currentLineLength());
    token.line = scanner.line;
    return token;
}


static Token emitErrorToken(const char *message) {
    Token token;
    token.tokenType = TOKEN_ERROR;
    token.start = message;
    token.length = strlen(message);
    token.line = scanner.line;
    return token;
}


static char advance() {
    scanner.current++;
    return scanner.current[-1];
}


static bool match(char expected) {
    if (isAtEnd() || (*scanner.current != expected)) return false;
    scanner.current++;
    return true;
}


static char peep() {
    return *scanner.current;
}


static char peepNext() {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}


static Token string() {
    while (peep() != '"' && !isAtEnd()) {
        if (peep() == '\n') scanner.line++;
        if (peep() == '\\' && peepNext() == '"') {
            // escaping ", skip on past
            advance(); 
            advance();
        }
        advance();
    }

    if (isAtEnd()) return emitErrorToken("Unterminated string.");
    advance(); // closing quote

    return emitToken(TOKEN_STRING);
}


static Token number() {
    while (isDigit(peep())) advance();

    // we support base 2, 8, 10, and 16 in literals
    if ((peep() == '.' || peep() == 'x' || peep() == 'o' || peep() == 'b') 
        && isDigit(peepNext())) {
        advance(); // nom nom the dot
        
        while(isDigit(peep())) advance();

        return emitToken(TOKEN_FLOAT);
    }

    return emitToken(TOKEN_NUMBER);
}


static TokenType checkKeyword(int start, int length, const char *rest, TokenType type) {
    if (currentLineLength() == start + length && 
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}


static TokenType identifierType() {

    switch(scanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "ny", TOKEN_ANY_TYPE);
        case 'b':
            if (currentLineLength() > 1) {
                switch (scanner.start[1]) {
                    case 'r': return checkKeyword(2, 3, "eak", TOKEN_BREAK);
                    case 'o': return checkKeyword(2, 2, "ol", TOKEN_BOOLEAN_TYPE);
                }
            }

            break;
        case 'c': return checkKeyword(1, 7, "ontinue", TOKEN_CONTINUE);
        case 'd': return checkKeyword(1, 2, "im", TOKEN_DIM);
        case 'e':
            if (currentLineLength() > 1) {
                switch (scanner.start[1]) {
                    case 'l': return checkKeyword(2, 2, "se", TOKEN_ELSE);
                    case 'n': return checkKeyword(2, 2, "um", TOKEN_ENUM);
                }
            }

            break;
        case 'f':
            if (currentLineLength() > 1) {
                switch (scanner.start[1]) {
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'u': return checkKeyword(2, 6, "nction", TOKEN_FUNCTION);
                }
            }

            break;
        case 'g': return checkKeyword(1, 3, "oto", TOKEN_GOTO);
        case 'i':
            if (currentLineLength() > 1) {
                switch (scanner.start[1]) {
                    case 'f': return TOKEN_IF;
                    case 'n': return checkKeyword(2, 5, "teger",TOKEN_INTEGER_TYPE);
                }
            }

            break;
        case 'l': return checkKeyword(1, 3, "ong", TOKEN_LONG_TYPE);
        case 'm': return checkKeyword(1, 4, "atch", TOKEN_MATCH);
        case 'n': return checkKeyword(1, 3, "one", TOKEN_NONE);
        case 'r':
            if (currentLineLength() > 1) {
                switch (scanner.start[1]) {
                    case 'e':
                        if (currentLineLength() > 2) {
                            switch (scanner.start[2]) {
                                case 't': return checkKeyword(3, 3, "urn", TOKEN_RETURN);
                                case 'a': return checkKeyword(3, 1, "l", TOKEN_REAL_TYPE);
                            }

                        }

                        break;
                }
            }

            break;
        case 's':
            if (currentLineLength() > 1) {
                switch (scanner.start[1]) {
                    case 'e': return checkKeyword(2, 2, "lf", TOKEN_SELF);
                    case 't':
                        if (currentLineLength() > 2) {
                            switch (scanner.start[2]) {
                                case 'e': return checkKeyword(3, 1, "p", TOKEN_STEP);
                                case 'r':
                                    if (currentLineLength() > 3) {
                                        switch (scanner.start[3]) {
                                            case 'i': return checkKeyword(4, 2, "ng", TOKEN_STRING_TYPE);
                                            case 'u': return checkKeyword(4, 2, "ct", TOKEN_STRUCT);
                                        }
                                    }

                                    break;
                            }
                        }

                        break;
                }
            }

            break;
        case 't': return checkKeyword(1, 3, "rue", TOKEN_TRUE);
        case 'u':
            if (currentLineLength() > 1) {
                switch (scanner.start[1]) {
                    case 's': return checkKeyword(2, 1, "e", TOKEN_USE);
                    case 'n':
                        if (currentLineLength() > 2) {
                            switch (scanner.start[2]) {
                                case 't': return checkKeyword(3, 2, "il", TOKEN_UNTIL);
                                case 's': return checkKeyword(3, 5, "igned", TOKEN_UNSIGNED);
                            }
                        }

                        break;
                }
            }

            break;
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}


static Token identifier() {
    while (isAlpha(peep()) || isDigit(peep())) advance();
    return emitToken(identifierType());
}


static void skipWhitespace() {
    for (;;) {
        switch (peep()) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peepNext() == '/') {
                    while (peep() != '\n' && !isAtEnd()) advance();
                }

                else if (peepNext() == '*') {
                    
                    // move past /*
                    advance();
                    advance();

                    while (!isAtEnd() && (peep() != '*' && peepNext() != '/')) {
                        if (peep() == '\n') scanner.line++;
                        advance();
                    }

                    advance();
                    advance();
                }
                
                break;
                
            default:
                return;
        }
    }
}


Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;

    if (isAtEnd()) return emitToken(TOKEN_EOF);
    
    char c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return emitToken(TOKEN_LEFT_PAREN);
        case ')': return emitToken(TOKEN_RIGHT_PAREN);
        case '[': return emitToken(TOKEN_LEFT_BRACE);
        case ']': return emitToken(TOKEN_RIGHT_BRACE);
        case '{': return emitToken(TOKEN_LEFT_CURLY_BRACE);
        case '}': return emitToken(TOKEN_RIGHT_CURLY_BRACE);
        case ',': return emitToken(TOKEN_COMMA);
        case '.': return emitToken(TOKEN_DOT);
        case '-': return emitToken(TOKEN_MINUS);
        case '+': return emitToken(TOKEN_PLUS);
        case ';': return emitToken(TOKEN_SEMICOLON);
        case '*': return emitToken(TOKEN_STAR);
        case '@': return emitToken(TOKEN_AT);
        case '/': return emitToken(TOKEN_SLASH);
        case '^': return emitToken(TOKEN_CARAT);
        case '~': return emitToken(TOKEN_TILDE);
        case '?': return emitToken(TOKEN_QUESTION);
        case '"': return string();

        case '!':
            return emitToken(
                match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG
            );
        case '=':
            return emitToken(
                match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL
            );
        case '>':
            return emitToken(
                match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER
            );
        case '<':
            return emitToken(
                match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS
            );
        case ':':
            return emitToken(
                match(':') ? TOKEN_COLON_COLON : TOKEN_COLON
            );
        case '&':
            return emitToken(
                match('&') ? TOKEN_LOGICAL_AND : TOKEN_AND
            );
        case '|':
            return emitToken(
                match('|') ? TOKEN_LOGICAL_OR : TOKEN_OR
            );
    }

    return emitErrorToken("Unexpected character.");
}
