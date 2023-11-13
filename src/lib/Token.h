#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum type {
    number,
    operator_,
    openParen,
    closeParen,
    identifier_,
    equal,
    boolTrue,
    boolFalse,
    lessThan,
    lessThanOrEqual,
    greaterThan,
    greaterThanOrEqual,
    equalEqual,
    notEqual,
    logicalAnd,
    logicalXor,
    logicalOr,
    openCurlyBracket,
    closeCurlyBracket,
    openBracket,
    closeBracket,
    ifStatement,
    elseStatement,
    whileStatement,
    printStatement,
    comma,
    semicolon,
    functionDefinitionStatement,
    returnStatement, 
    null,
    end
};

struct Token {
    type type_;
    int line;
    int column;
    std::string value;

    Token(type type_, int line, int column, std::string value) {
        this->type_ = type_;
        this->line = line;
        this->column = column;
        this->value = value;
    }
};

#endif
