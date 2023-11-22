#ifndef FUNCTION_H
#define FUNCTION_H

#include "Token.h"

#include <memory>
#include <iostream>
#include <vector>
#include <variant>

class FunctionDefNode;

class Function {
    public:
    FunctionDefNode* node;
    std::vector<Token> parameters;
    std::vector<Token> block;

    Function(FunctionDefNode* n, std::vector<Token> p, std::vector<Token> b) : node(n), parameters(p), block(b) {
    }
};


#endif