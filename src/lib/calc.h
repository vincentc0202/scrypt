#ifndef CALC_H
#define CALC_H

#include "Token.h" 
#include "lex.h"
#include "Value.h"

#include <map>
#include <stdexcept>
#include <cmath>


class ASTNode {
public:
    std::string type;

    virtual ~ASTNode() = default;
    virtual Value evaluate() { return 0.0; }
    virtual void printInfix() = 0;
};
class UnexpToken : public std::runtime_error {
public:
    UnexpToken(const std::string& error) : std::runtime_error(error) {}
};

extern std::map<std::string, Value> symbTable;

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

        if (std::holds_alternative<double>(val->second)) {
            return std::get<double>(val->second);
        }
        else {
            return std::get<bool>(val->second);
        }
    }

    void printInfix() override {
        std::cout << name;
    }

    const std::string& getName() const {
        return name;
    }
};


class NumberNode : public ASTNode {
    double value;

public:
    NumberNode(double val) : value(val) {
        type = "number";
    }
    void printInfix() override {
        std::cout << value;
    }
    
    Value evaluate() override {
        return value;
    }
};

class BooleanNode : public ASTNode {
    bool value;

    public: 
        BooleanNode(bool val) : value(val) {
            type = "boolean";
        }

        Value evaluate() override {
            if (value) {
                return true;
            }
            else {
                return false;
            }
        }

        void printInfix() override {
            std::cout << (value ? "true" : "false");
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
        if (std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate())) {
            return std::get<double>(left->evaluate()) + std::get<double>(right->evaluate());
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
        if (std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate())) {
            return std::get<double>(left->evaluate()) - std::get<double>(right->evaluate());
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
        if (std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate())) {
            return std::get<double>(left->evaluate()) * std::get<double>(right->evaluate());
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
        double rightEval = std::get<double>(right->evaluate());
        if (rightEval == 0) {
            throw std::runtime_error("division by zero.");
        }
        if (std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate())) {
            return std::get<double>(left->evaluate()) / rightEval;
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
        double rightEval = std::get<double>(right->evaluate());
        if (rightEval == 0) {
            throw std::runtime_error("division by zero.");
        }
        if (std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate())) {
            return std::fmod(std::get<double>(left->evaluate()), rightEval);
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
        if ((std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate()))) {
            return (left->evaluate() < right->evaluate());
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
        if ((std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate()))) {
            return (left->evaluate() <= right->evaluate());
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
        if ((std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate()))) {
            return (left->evaluate() > right->evaluate());
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
        if ((std::holds_alternative<double>(left->evaluate()) && std::holds_alternative<double>(right->evaluate()))) {
            return (left->evaluate() >= right->evaluate());
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
        return (left->evaluate() == right->evaluate());
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
        return (left->evaluate() != right->evaluate());
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
        if ((std::holds_alternative<bool>(left->evaluate()) && std::holds_alternative<bool>(right->evaluate()))) {
            return (std::get<bool>(left->evaluate()) && std::get<bool>(right->evaluate()));
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
        if ((std::holds_alternative<bool>(left->evaluate()) && std::holds_alternative<bool>(right->evaluate()))) {
            return (std::get<bool>(left->evaluate()) != std::get<bool>(right->evaluate()));
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
        if ((std::holds_alternative<bool>(left->evaluate()) && std::holds_alternative<bool>(right->evaluate()))) {
            return (std::get<bool>(left->evaluate()) || std::get<bool>(right->evaluate()));
        }
        else {
            throw std::runtime_error("invalid operand type.");
        }
    }

    void printInfixOp() const override {
        std::cout << " | ";
    }
};

class FunctionNode : public ASTNode {
    std::string name;
    std::vector<VariableNode> parameters;
    std::vector<Token> statementBlock;

    public:
    FunctionNode(std::string n, std::vector<VariableNode> param, std::vector<Token> block) : name(n), parameters(param), statementBlock(block) {
        type == "function";
    }
    


};

class ReturnNode : public ASTNode {
    std::vector<ASTNode> returnExpression;

    public:
    ReturnNode(std::vector<ASTNode> returnExpression) {
        
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