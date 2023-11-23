#ifndef SCRYPT_H
#define SCRYPT_H

#include "lex.h"
#include "Value.h"
#include "Function.h"
// #include "calc.h"

#include <map>
#include <algorithm>

class ASTNode;
class ReturnNode;

struct Scrypt {
    void interpret(std::vector<Token>& tokens, Value& returnNodeValue);
    
private:
    void deleteBlock(std::vector<Token>& tokens);
    std::vector<Token> getExpression(std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> evalExpression(std::vector<Token>& tokens);
};

#endif