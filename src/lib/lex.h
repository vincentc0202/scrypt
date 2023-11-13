#ifndef LEX_H
#define LEX_H

#include "Token.h"

#include <vector>
#include <iostream>

class lex {
public:
    //returns an array of tokens; this is where the logic is applied
    std::vector<Token> tokenize(std::string input);
    void printTokens(std::vector<Token> tokens);
};

#endif