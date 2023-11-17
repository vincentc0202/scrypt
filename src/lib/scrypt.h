#ifndef SCRYPT_H
#define SCRYPT_H

#include "Token.h" 
#include "lex.h"
#include "calc.h"
#include <map>
#include <algorithm>
#include "Value.h"


struct Scrypt {
    std::map<std::string, std::shared_ptr<FunctionNode>> functionDef;

    void interpret(std::vector<Token>& tokens);
    
private:
    //std::vector<std::shared_ptr<FunctionNode>> functionList;
    void deleteBlock(std::vector<Token>& tokens);
    std::vector<Token> getExpression(std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> evalExpression(std::vector<Token>& tokens);
};

#endif