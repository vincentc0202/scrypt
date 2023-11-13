#ifndef PARSE_H
#define PARSE_H

#include "Token.h" 
#include "lex.h"

#include <unordered_map>

struct ASTNode {
    Token token;
    std::vector<ASTNode> children;

    ASTNode (Token token) : token(token) {}

}; 

class Parser{
public: 
    size_t index;
    double result;
    std::unordered_map<std::string, double> identifiers;

    Parser() : index(0), result(0.0) {}
    ASTNode parseExpression(std::vector<Token>& tokens, size_t& index, std::unordered_map<std::string, double>& identifiers);
    void printInfix(ASTNode& node);
    double evaluate(ASTNode& node, double result, std::unordered_map<std::string, double>& identifiers);

};


#endif // PARSE_H