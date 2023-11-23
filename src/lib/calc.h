#ifndef CALC_H
#define CALC_H

#include "lex.h"
#include "Value.h"
#include "Function.h"
#include "scrypt.h"
#include "format.h"

#include <map>
#include <stdexcept>
#include <cmath>
#include <algorithm>

extern std::map<std::string, Value> symbTable;

class UnexpToken : public std::runtime_error {
public:
    UnexpToken(const std::string& error) : std::runtime_error(error) {}
};

class ASTNode {
public:
    std::string type;

    virtual ~ASTNode() = default;
    virtual Value evaluate() = 0;
    virtual void printInfix() = 0;
};

class VariableNode : public ASTNode {
    std::string name;
public:
    VariableNode(const std::string& n) : name(n) {
        type = "variable";
    }

    Value evaluate() override {
        auto val = symbTable.find(name);
        if (val == symbTable.end()) {
            throw std::runtime_error("unknown identifier " + name);
        }

        Value second = val->second;
        
        if (std::holds_alternative<double>(second)) {
            return std::get<double>(second);
        }
        else if (std::holds_alternative<bool>(second)){
            return std::get<bool>(second);
        }
        else if (std::holds_alternative<FunctionPtr>(second)) {
            return std::get<FunctionPtr>(second);
        }
        else {
            throw std::runtime_error("oog");
        }
        // else if (std::holds_alternative<Array>(second)){
        //     return std::get<Array>(second);
        // }
        // else if (std::holds_alternative<Null>(second)){
        //     return std::get<Null>(second);
        // }
    }

    void printInfix() override {
        std::cout << name;
    }

    const std::string& getName() const {
        return name;
    }
};


class NumberNode : public ASTNode {
    Value value;

public:
    NumberNode(Value val) : value(val) {
        type = "number";
    }
    void printInfix() override {
        std::cout << std::get<double>(value);
    }
    
    Value evaluate() override {
        return std::get<double>(value);
    }
};

class BooleanNode : public ASTNode {
    Value value;

    public: 
        BooleanNode(Value val) : value(val) {
            type = "boolean";
        }

        Value evaluate() override {
            if (std::get<bool>(value)) {
                return std::get<bool>(value);
            }
            else {
                return std::get<bool>(value);
            }
        }

        void printInfix() override {
            std::cout << (std::get<bool>(value) ? "true" : "false");
        }
};

class AssignNode : public ASTNode {
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
public:
    AssignNode(ASTNode* l, ASTNode* r) : left(l), right(r) {
        type = "assignment";
    }

    Value evaluate() override {
        if (left->type != "variable") {
            throw std::runtime_error("invalid assignee.");
        }

        auto leftName = dynamic_cast<VariableNode*>(left.get());
        Value rhs = right->evaluate();

        symbTable[leftName->getName()] = rhs;
        return rhs;
    }

    void printInfix() override {
        std::cout << "(";
        left->printInfix();
        std::cout << " = ";
        right->printInfix();
        std::cout << ")";
    }

};

class BinaryOpNode : public ASTNode {
protected:
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

public:
    BinaryOpNode(ASTNode* l, ASTNode* r) : left(l), right(r) {
        type = "binaryOperator";
    }
    
    virtual void printInfixOp() const = 0;

    void printInfix() override {
        std::cout << "(";
        left->printInfix();
        printInfixOp();
        right->printInfix();
        std::cout << ")";
    }
};

class AddNode : public BinaryOpNode {
public:
    AddNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) { 
        type = "arithmeticOperator"; 
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
            return std::get<double>(lhs) + std::get<double>(rhs);
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " + ";
    }
};

class SubtractNode : public BinaryOpNode {
public:
    SubtractNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "arithmeticOperator"; 
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
            return std::get<double>(lhs) - std::get<double>(rhs);
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " - ";
    }
};

class MultiplyNode : public BinaryOpNode {
public:
    MultiplyNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "arithmeticOperator"; 
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
            return std::get<double>(lhs) * std::get<double>(rhs);
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " * ";
    }
};

class DivideNode : public BinaryOpNode {
public:
    DivideNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "arithmeticOperator"; 
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        double rightEval = std::get<double>(rhs);

        if (rightEval == 0) {
            throw std::runtime_error("division by zero.");
        }
        if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
            return std::get<double>(lhs) / rightEval;
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " / ";
    }
};

class ModuloNode : public BinaryOpNode {
public:
    ModuloNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "arithmeticOperator"; 
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        double leftEval = std::get<double>(lhs);
        double rightEval = std::get<double>(rhs);
        
        if (rightEval == 0) {
            throw std::runtime_error("division by zero.");
        }
        if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
            double result = std::fmod(leftEval, rightEval);
            Value val;
            val.emplace<double>(result);
            return val;
        }   
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " % ";
    }
};

class LessThanNode : public BinaryOpNode {
public:
    LessThanNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
            return (std::get<double>(lhs) < std::get<double>(rhs));
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " < ";
    }
};

class LessThanOrEqualNode : public BinaryOpNode {
public:
    LessThanOrEqualNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();

        if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
            return (std::get<double>(lhs) <= std::get<double>(rhs));
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " <= ";
    }
};

class GreaterThanNode : public BinaryOpNode {
public:
    GreaterThanNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }
    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
            return (std::get<double>(lhs) > std::get<double>(rhs));
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " > ";
    }
};

class GreaterThanOrEqualNode : public BinaryOpNode {
public:
    GreaterThanOrEqualNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if ((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))) {
            return (std::get<double>(lhs) >= std::get<double>(rhs));
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " >= ";
    }
};

class EqualityNode : public BinaryOpNode {
public:
    EqualityNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        return (std::get<double>(lhs) == std::get<double>(rhs));
    }

    void printInfixOp() const override {
        std::cout << " == ";
    }
};

class InequalityNode : public BinaryOpNode {
public:
    InequalityNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        return (std::get<double>(lhs) != std::get<double>(rhs));
    }

    void printInfixOp() const override {
        std::cout << " != ";
    }
};

class LogicalAndNode : public BinaryOpNode {
public:
    LogicalAndNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if ((std::holds_alternative<bool>(lhs) && std::holds_alternative<bool>(rhs))) {
            return (std::get<bool>(lhs) && std::get<bool>(rhs));
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " & ";
    }
};

class LogicalXORNode : public BinaryOpNode {
public:
    LogicalXORNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if ((std::holds_alternative<bool>(lhs) && std::holds_alternative<bool>(rhs))) {
            return (std::get<bool>(lhs) != std::get<bool>(rhs));
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " ^ ";
    }
};

class LogicalInclusiveOrNode : public BinaryOpNode {
public:
    LogicalInclusiveOrNode(ASTNode* l, ASTNode* r) : BinaryOpNode(l, r) {
        type = "boolOperator";
    }

    Value evaluate() override {
        Value lhs = left->evaluate();
        Value rhs = right->evaluate();
        if ((std::holds_alternative<bool>(lhs) && std::holds_alternative<bool>(rhs))) {
            return (std::get<bool>(lhs) || std::get<bool>(rhs));
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " | ";
    }
};

class ReturnNode : public ASTNode {
    public:
    std::unique_ptr<ASTNode> returnExpression;
    ReturnNode(std::unique_ptr<ASTNode> r) : returnExpression(std::move(r)) {
    }

    Value evaluate() override{
        if (returnExpression == nullptr) {
            return nullptr;
        }
        else {
            return returnExpression->evaluate();
        }
    }

    void printInfix() override{
        if (returnExpression == nullptr) {
            std::cout << "return;\n";
        }
        else {
            std::cout << "return ";
            returnExpression->printInfix();
            std::cout << ";\n";
        }
    }
};

class NullNode : public ASTNode {
    Value value;

public:
    NullNode(Value val) : value(val) {
        type = "null";
    }
    void printInfix() override {
        std::cout << "null";
    }
    
    Value evaluate() override {
        return nullptr;
    }
};

class FunctionDefNode : public ASTNode {
    public:
    std::string name;
    std::vector<Token> parameters;
    std::vector<Token> block;
    Format format;

    FunctionDefNode(std::string n, std::vector<Token> param, std::vector<Token> b) : name(n), parameters(param), block(b) {
        type = "functionDef";
    }
    
    Value evaluate() override{
        return symbTable[name];
    }
    void printInfix(int& curlyCounter) {
        std::cout << name << "(";
        if (parameters.size() == 0) {
            std::cout << ") {\n";
        }
        else if (parameters.size() == 1) {
            std::cout << parameters[0].value << ") {\n";
        }
        else {
            for (size_t i = 0; i < parameters.size() - 1; i++) {
                std::cout << parameters[i].value << ", ";
            }
            std::cout << parameters[parameters.size() - 1].value << ") {\n";
        }

        //print out block
        std::reverse(block.begin(), block.end());
        format.printFormat(block, ++curlyCounter);
        format.printIndents(--curlyCounter);
        std::cout << "}\n";
    }
    void printInfix() override {}
};

class FunctionCallNode : public ASTNode {
    public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> arguments;
    Scrypt scrypt;

    FunctionCallNode(std::string n, std::vector<std::unique_ptr<ASTNode>> args) : name(n), arguments(std::move(args)) {
        type = "functionCall";
    }
    
    Value evaluate() override {
        std::map<std::string, Value> globalScope = symbTable;
        std::vector<Value> args;

        auto functionName = symbTable.find(name);
        if (functionName == symbTable.end()) {
            throw std::runtime_error("not a function.");
        }

        //evaluate arguments first
        for (size_t i = 0; i < arguments.size(); i++) {
            Value arg = arguments[i]->evaluate();
            args.push_back(arg);
        }

        FunctionPtr function = std::get<FunctionPtr>(symbTable[name]);

        if (arguments.size() != function->parameters.size()) {
            throw std::runtime_error("incorrect argument count.");
        }
        for (size_t i = 0; i < arguments.size(); i++) {
            symbTable[function->parameters[i].value] = args[i];
        }
        //still unsure about this
        // ReturnNode*& returnNode(nullptr);
        std::unique_ptr<ReturnNode> returnNode(nullptr);
        std::reverse(function->block.begin(), function->block.end());
        scrypt.interpret(function->block, std::move(returnNode));

        symbTable = globalScope;

        if (returnNode == nullptr) {
            return nullptr;
        }
        else {
            Value result = returnNode->evaluate();
            //reset back to null for nested loops
            returnNode->returnExpression = nullptr;
            return result;
        }

        // symbTable = globalScope;
        // return function;
    }
    void printInfix() override{
        std::cout << name << "(";
        if (arguments.size() == 1) {
            arguments[0]->printInfix();
        }
        else if (arguments.size() > 1) {
            for (size_t i = 0; i < arguments.size() - 1; i++) {
                arguments[i]->printInfix();
                std::cout << ", ";
            }
            arguments[arguments.size() - 1]->printInfix();
        }
        std::cout << ")";
    }
};



class Parser {
 
public:
    int parencount = 0;
    std::unique_ptr<ASTNode> parseExpression(const std::vector<Token>& tokens, size_t& pos);
    std::unique_ptr<ASTNode> parseInclusiveOr(const std::vector<Token>& tokens, size_t& pos);
    std::unique_ptr<ASTNode> parseExclusiveOr(const std::vector<Token>& tokens, size_t& pos);
    std::unique_ptr<ASTNode> parseLogicalAnd(const std::vector<Token>& tokens, size_t& pos);
    std::unique_ptr<ASTNode> parseEquality(const std::vector<Token>& tokens, size_t& pos);
    std::unique_ptr<ASTNode> parseComparison(const std::vector<Token>& tokens, size_t& pos);
    std::unique_ptr<ASTNode> parseAddSub(const std::vector<Token>& tokens, size_t& pos);
    std::unique_ptr<ASTNode> parseMultDivMod(const std::vector<Token>& tokens, size_t& pos);
    std::unique_ptr<ASTNode> parseFactor(const std::vector<Token>& tokens, size_t& pos);
};

#endif // CALC_H