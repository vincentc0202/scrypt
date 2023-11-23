#include "calc.h"

std::map<std::string, Value> symbTable;

std::unique_ptr<ASTNode> Parser::parseExpression(const std::vector<Token>& tokens, size_t& pos) {
    
    // passes tokens to Addsub -> MultDiv -> Factor (lowest to highest precedence)
    auto left = parseInclusiveOr(tokens, pos);
    
    if (pos < tokens.size() && tokens[pos].type_ == equal){
        pos++; // skip the assignment op
        if (tokens[pos].type_ != end) {
            // right-associative behavior
            auto right = parseExpression(tokens, pos); 

            // Create a new assignment node with the copied LHS
            return std::make_unique<AssignNode>(left.release(), right.release());
        }
        else {
            throw UnexpToken("Unexpected token at line " + std::to_string(tokens[pos].line) + " column " + std::to_string(tokens[pos].column) + ": " + tokens[pos].value);
        }
    }
    
    // conditionals 
    return left;
}

std::unique_ptr<ASTNode> Parser::parseInclusiveOr(const std::vector<Token>& tokens, size_t& pos) {
    auto left = parseExclusiveOr(tokens, pos);

    while (pos < tokens.size() && (tokens[pos].value == "|")) {
        std::string op = tokens[pos].value;
        pos++;
        auto right = parseExclusiveOr(tokens, pos);

        if(op == "|") {
            left = std::make_unique<LogicalInclusiveOrNode>(left.release(), right.release());
        } 
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseExclusiveOr(const std::vector<Token>& tokens, size_t& pos) {
    auto left = parseLogicalAnd(tokens, pos);

    while (pos < tokens.size() && (tokens[pos].value == "^")) {
        std::string op = tokens[pos].value;
        pos++;
        auto right = parseLogicalAnd(tokens, pos);

        if(op == "^") {
            left = std::make_unique<LogicalXORNode>(left.release(), right.release());
        } 
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseLogicalAnd(const std::vector<Token>& tokens, size_t& pos) {
    auto left = parseEquality(tokens, pos);

    while (pos < tokens.size() && (tokens[pos].value == "&")) {
        std::string op = tokens[pos].value;
        pos++;
        auto right = parseEquality(tokens, pos);

        if(op == "&") {
            left = std::make_unique<LogicalAndNode>(left.release(), right.release());
        } 
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseEquality(const std::vector<Token>& tokens, size_t& pos) {
    auto left = parseComparison(tokens, pos);

    while (pos < tokens.size() && (tokens[pos].value == "==" || tokens[pos].value == "!=")) {
        std::string op = tokens[pos].value;
        pos++;
        auto right = parseComparison(tokens, pos);

        if(op == "==") {
            left = std::make_unique<EqualityNode>(left.release(), right.release());
        } 
        else if (op == "!=") {
            left = std::make_unique<InequalityNode>(left.release(), right.release());
        } 
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseComparison(const std::vector<Token>& tokens, size_t& pos) {
    auto left = parseAddSub(tokens, pos);

    while (pos < tokens.size() && (tokens[pos].value == "<" || tokens[pos].value == "<=" || tokens[pos].value == ">" || tokens[pos].value == ">=")) {
        std::string op = tokens[pos].value;
        pos++;
        auto right = parseAddSub(tokens, pos);

        if(op == "<") {
            left = std::make_unique<LessThanNode>(left.release(), right.release());
        } 
        else if(op == "<=") {
            left = std::make_unique<LessThanOrEqualNode>(left.release(), right.release());
        } 
        else if(op == ">") {
            left = std::make_unique<GreaterThanNode>(left.release(), right.release());
        } 
        else if(op == ">=") {
            left = std::make_unique<GreaterThanOrEqualNode>(left.release(), right.release());
        } 
    }
    return left;
}

//Deals with addition and subtraction
std::unique_ptr<ASTNode> Parser::parseAddSub(const std::vector<Token>& tokens, size_t& pos){
    auto left = parseMultDivMod(tokens,pos);

    while(pos < tokens.size() && (tokens[pos].type_ == operator_ && (tokens[pos].value == "+" || tokens[pos].value == "-"))) {
        std::string op = tokens[pos].value;
        pos++;
        auto right = parseMultDivMod(tokens, pos);

        if(op == "+"){
            left = std::make_unique<AddNode>(left.release(), right.release());
        } 
        else if (op == "-"){
            left = std::make_unique<SubtractNode>(left.release(), right.release());
        }
    }
    return left;
}

//Deals with multiplication, division, and modulo operators
std::unique_ptr<ASTNode> Parser::parseMultDivMod(const std::vector<Token>& tokens, size_t& pos) {
    auto left = parseFactor(tokens, pos);
    while(pos < tokens.size() && (tokens[pos].type_ == operator_ && (tokens[pos].value == "*" || tokens[pos].value == "/" || tokens[pos].value == "%"))) {
        std::string op = tokens[pos].value;
        pos++;
        auto right = parseFactor(tokens, pos);

        if(op == "*"){
            left = std::make_unique<MultiplyNode>(left.release(), right.release());
        } 
        else if(op == "/"){
            left = std::make_unique<DivideNode>(left.release(), right.release());
        }
        else if (op == "%") {
            left = std::make_unique<ModuloNode>(left.release(), right.release());
        }
    }
    return left;
}

//Deals with numbers, identifiers, and parentheses
std::unique_ptr<ASTNode> Parser::parseFactor(const std::vector<Token>& tokens, size_t& pos){
    //auto left = parseFunctionDef(tokens, pos);
    if (pos >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }

    if (tokens[pos].type_ == number) {
        double value = std::stod(tokens[pos].value);
        pos++;

        //parencount checker
        if(pos < tokens.size() && tokens[pos].type_ == openParen){
            parencount++;
        } 
        else if (pos < tokens.size() && tokens[pos].type_ == closeParen){
            parencount--;
        }
        return std::make_unique<NumberNode>(value);
    } 
    else if (tokens[pos].type_ == null) {
        pos++;
        return std::make_unique<NullNode>(nullptr);
    }
    else if (tokens[pos].type_ == identifier_) {
        std::string varName = tokens[pos].value;
        pos++;

        //has to be function call here
        if (pos < tokens.size() && tokens[pos].type_ == openParen){
            //process arguments
            std::vector<std::unique_ptr<ASTNode>> arguments;
            //skip (
            pos++;
            while (pos < tokens.size() && tokens[pos].type_ != closeParen) {
                std::vector<Token> tempArgument;
                while (pos < tokens.size() && tokens[pos].type_ != comma && tokens[pos].type_ != closeParen) {
                    tempArgument.push_back(tokens[pos]);
                    pos++;
                }

                //process comma if there is one
                if (pos < tokens.size() && tokens[pos].type_ == comma) {
                    pos++;
                    if (pos < tokens.size() && tokens[pos].type_ == closeParen) {
                        throw UnexpToken("Unexpected token at line " + std::to_string(tokens[pos-1].line) + " column " + std::to_string(tokens[pos-1].column) + ": " + tokens[pos-1].value);
                    }
                }

                size_t tempPos = 0;
                auto argumentPointer = parseExpression(tempArgument, tempPos);
                arguments.push_back(std::move(argumentPointer));  
            }
            //skip )
            pos++;

            return std::make_unique<FunctionCallNode>(varName, std::move(arguments), symbTable);
        }

        if (pos < tokens.size() - 1 && tokens[pos].type_ == closeParen){
            parencount--;
        }

        return std::make_unique<VariableNode>(varName);
    } 
    else if (tokens[pos].type_ == boolTrue || tokens[pos].type_ == boolFalse) {
        std::string value = tokens[pos].value;
        pos++;

        //parencount checker
        if(pos < tokens.size() && tokens[pos].type_ == openParen){
            parencount++;
        } 
        else if (pos < tokens.size() && tokens[pos].type_ == closeParen){
            parencount--;
        }

        if (value == "true") {
            return std::make_unique<BooleanNode>(true);
        }
        else {
            return std::make_unique<BooleanNode>(false);
        }
    }
    else if (tokens[pos].type_ == openParen) {
        pos++; // skip opening parentheses
        parencount++;
    
        auto node = parseExpression(tokens, pos);
        if (pos >= tokens.size() || tokens[pos].type_ != closeParen) {
            throw UnexpToken("Unexpected token at line " + std::to_string(tokens[pos].line) + " column " + std::to_string(tokens[pos].column) + ": " + tokens[pos].value);
        }
        pos++; // skip closing parentheses

        if(pos < tokens.size() && tokens[pos].type_ == closeParen){
            parencount--;
        }
        return node;
    } 
    else {
        throw UnexpToken("Unexpected token at line " + std::to_string(tokens[pos].line) + " column " + std::to_string(tokens[pos].column) + ": " + tokens[pos].value);
    }
}

