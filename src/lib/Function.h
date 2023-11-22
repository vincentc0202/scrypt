#ifndef FUNCTION_H
#define FUNCTION_H

#include "Token.h"

#include <memory>
#include <iostream>
#include <vector>
#include <variant>
#include <map>

class FunctionDefNode;

class Function {
    public:
    FunctionDefNode* node;
    std::vector<Token> parameters;
    std::vector<Token> block;
    std::map<std::string, Value> localScope;

    // Constructor implementation
    Function(FunctionDefNode* n, std::vector<Token> p, std::vector<Token> b, const std::map<std::string, Value>& currentScope)
        : node(n), parameters(p), block(b), localScope(currentScope) {
        
    }
};


#endif