#include "parser/ast/ast.h"
#include "common.h"

struct AstNode *makeAstNode(Token *token, struct AstNode *leftNode, struct AstNode *rightNode) {
    struct AstNode *newNode = (struct AstNode *)malloc(sizeof(struct AstNode));

    newNode->token = token;
    newNode->lhs = leftNode;
    newNode->rhs = rightNode;

    return newNode;
}


struct AstNode *makeAstLeaf(Token *token) {
    return makeAstNode(token, NULL, NULL);
}


struct AstNode *makeAstUnary(Token *token, struct AstNode *leftNode) {
    return makeAstNode(token, leftNode, NULL);
}