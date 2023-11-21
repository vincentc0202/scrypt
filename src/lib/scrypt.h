#ifndef SCRYPT_H
#define SCRYPT_H

#include "lex.h"
#include "calc.h"
#include "Value.h"
#include "Function.h"

#include <map>
#include <algorithm>

class ASTNode;

struct Scrypt {
    void interpret(std::vector<Token>& tokens);
    
private:
    void deleteBlock(std::vector<Token>& tokens);
    std::vector<Token> getExpression(std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> evalExpression(std::vector<Token>& tokens);
};

#endif