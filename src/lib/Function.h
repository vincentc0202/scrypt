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
    std::unique_ptr<FunctionDefNode> node;
    std::vector<Token> parameters;
    std::vector<Token> block;

    Function(std::unique_ptr<FunctionDefNode> n, std::vector<Token> p, std::vector<Token> b) : node(std::move(n)), parameters(p), block(b) {
    }
};


#endif