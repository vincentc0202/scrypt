#ifndef SCRYPT_H
#define SCRYPT_H

#include "Token.h" 
#include "lex.h"
#include "calc.h"

#include <algorithm>


struct Scrypt {
    void interpret(std::vector<Token>& tokens);

private:
    void deleteBlock(std::vector<Token>& tokens);
    std::vector<Token> getExpression(std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> evalExpression(std::vector<Token>& tokens);
};

// struct StatementNode {
//     std::unique_ptr<BinaryOpNode> expression;

//     virtual ~StatementNode() = default;
//     //virtual void printInfixStatement() const = 0;
//     virtual Value evalExpression() {return false;}
//     virtual Value evalBlock() { return 0.0; }
//      //virtual void printExpression = 0;
//      //virtual void printBlock = 0;
// };

// struct IfNode : public StatementNode {
//      std::unique_ptr<std::vector<ASTNode>> block;
     
//     IfNode(BinaryOpNode* boolE, std::vector<ASTNode>* block) {
//         this->expression = boolE;
//         this->block = block;
//     }

// };

// struct ElseNode : public StatementNode {
//      std::unique_ptr<ASTNode> block;
//     //constructor

//     void printInfixStatement() const override {
//         std::cout << "else ";
//     }
// };

// struct WhileNode : public StatementNode {
//      std::unique_ptr<ASTNode> block;
//     //constructor

//     void printInfixStatement() const override {
//         std::cout << "while ";
//     }
// };

// struct PrintNode : public StatementNode {
// 
//     //constructor

//     void printInfixStatement() const override {
//         std::cout << "print ";
//     }
// };


#endif