#ifndef SCRYPT_H
#define SCRYPT_H

#include "Token.h" 
#include "lex.h"
#include "calc.h"
#include <map>
#include <algorithm>
#include "Value.h"
#include "Function.h"


struct Scrypt {
    // std::map<std::string, Function> functionDef;

    void interpret(std::vector<Token>& tokens);
    
private:
    //std::vector<Function> functionList;
    void deleteBlock(std::vector<Token>& tokens);
    std::vector<Token> getExpression(std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> evalExpression(std::vector<Token>& tokens);
};

#endif