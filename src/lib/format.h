#ifndef FORMAT_H
#define FORMAT_H

#include "Token.h" 
#include "lex.h"

#include <algorithm>
#include <memory>

// class ReturnNode;

struct Format {
    void printIndents(int counter);
    void printFormat(std::vector<Token>& tokens, int curlyCounter = 0);
};

#endif