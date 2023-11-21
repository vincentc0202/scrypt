#ifndef FUNCTION_H
#define FUNCTION_H

#include "calc.h"
#include "Token.h"

#include <memory>
#include <iostream>
#include <vector>
#include <variant>

class Function {
    public:
    std::unique_ptr<FunctionDefNode> node;
    std::vector<Token> parameters;
    std::vector<Token> block;

    Function(std::unique_ptr<FunctionDefNode> n, std::vector<Token> p, std::vector<Token> b) : node(std::move(n)), parameters(std::move(p)), block(std::move(b)) {
    }

    // Value getValue(std::vector<Value> arguments, std::map<std::string, Value> variables) {

    // }


};


#endif