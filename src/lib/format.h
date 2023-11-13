#ifndef FORMAT_H
#define FORMAT_H

#include "Token.h" 
#include "lex.h"
#include "calc.h"

#include <algorithm>

struct Format {
    int curlyCounter = 0;
    void printFormat(std::vector<Token>& tokens);
};

#endif