#ifndef CPIPP_AST_H
#define CPIPP_AST_H


#include "lexer/token.h"


struct AstNode {
    Token *token;
    struct AstNode *lhs;
    struct AstNode *rhs;
};

struct AstNode *makeAstNode(Token *token, struct AstNode *leftNode, struct AstNode *rightNode);
struct AstNode *makeAstLeaf(Token *token);
struct AstNode *makeAstUnary(Token *token, struct AstNode *leftNode);

#endif