#ifndef FORMAT_H
#define FORMAT_H

#include "Token.h" 
#include "lex.h"

#include <algorithm>

struct Format {
    int curlyCounter = 0;
    
    void printIndents(int counter);
    void printFormat(std::vector<Token>& tokens, int curlyCounter = 0);
};

#endif