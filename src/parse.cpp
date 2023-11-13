#include "lib/parse.h"

ASTNode Parser::parseExpression(std::vector<Token>& tokens, size_t& index, std::unordered_map<std::string, double>& identifiers) {

    ASTNode currNode = ASTNode(tokens[index]);
    
    if(currNode.token.type_ == number) {
        //multiple expression w/ individual number first
        if (currNode.token.line == 2 && currNode.token.column == 2 && tokens[++index].type_ != end) {
            std::cout << "EUnexpected token at line " << tokens[index].line << " column " << tokens[index].column << ": " << tokens[index].value << "\n";
            throw 2;

        }
        return currNode;
    }
    else if (currNode.token.type_ == identifier_) {
        return currNode;
    }
    else if(tokens[index].type_ == openParen) {
        ASTNode opNode = ASTNode(tokens[++index]);

        if (opNode.token.type_ != operator_ && opNode.token.type_ != equal) {
            std::cout << "Unexpected token at line " << opNode.token.line << " column " << opNode.token.column << ": " << opNode.token.value << "\n";
            throw 2;
        }
        //populating the operator's children
        while(tokens[++index].type_ != closeParen) {
            opNode.children.push_back(parseExpression(tokens, index, identifiers));
        }

        //check edge cases, no evaluation logic here
        if (opNode.children.empty()) {
            std::cout << "Unexpected token at line " << tokens[index].line << " column " << tokens[index].column << ": " << tokens[index].value << "\n";
            throw 2;
        }
        if (opNode.token.type_ == equal) {
            //if first child is not an identifier, return that child
            if (opNode.children[0].token.type_ != identifier_) {
                int newIndex = index - opNode.children.size();
                if (opNode.children[0].token.type_ == operator_) {
                    std::cout << "Unexpected token at line " << opNode.children[0].token.line << " column " << opNode.children[0].token.column - 1 << ": (\n";
                    throw 2;
                }
                else {
                    std::cout << "Unexpected token at line " << tokens[newIndex].line << " column " << tokens[newIndex].column << ": " << tokens[newIndex].value << "\n";
                    throw 2;
                }
            }
            //if there are less than two children return )
            if (opNode.children.size() < 2) {
                std::cout << "Unexpected token at line " << tokens[index].line << " column " << tokens[index].column << ": " << tokens[index].value << "\n";
                throw 2;
            }
            //if one of the middle children are not identifiers, return the last child
            for (size_t i = 0; i < opNode.children.size() - 1; i++) {
                if (opNode.children[i].token.type_ != identifier_) {
                    std::cout << "Unexpected token at line " << tokens[index-1].line << " column " << tokens[index-1].column << ": " << tokens[index-1].value << "\n";
                    throw 2;
                }  
            }
        }
        return opNode;
    }
    else {
        std::cout << "Unexpected token at line " << tokens[index].line << " column " << tokens[index].column << ": " << tokens[index].value << "\n";
        throw 2;
    }
}

void Parser::printInfix(ASTNode& node) {
    if (node.token.type_ == operator_ || node.token.type_ == equal) {
        std::string op = node.token.value;
        std::cout << "(";
        for (size_t i = 0; i < node.children.size(); i++) {
            printInfix(node.children.at(i));

            if (i < node.children.size() - 1) {
                std::cout << " " << op << " ";
            }
        }
        std::cout << ")";
    }
    else if (node.token.type_ == number) {
        printf("%.6g", std::stod(node.token.value));
    }
    else if (node.token.type_ == identifier_) {
        std::cout << node.token.value;
    }

}

double Parser::evaluate(ASTNode& node, double result, std::unordered_map<std::string, double>& identifiers) {
    if (node.token.type_ == operator_ || node.token.type_ == equal) {
        std::string op = node.token.value;
        for (size_t i = 0; i < node.children.size(); i++) {
            if (op == "+") {
                i == 0 ? result = evaluate(node.children.at(i), result, identifiers) : result += evaluate(node.children.at(i), result, identifiers);
            }
            else if (op == "-") {
                i == 0 ? result = evaluate(node.children.at(i), result, identifiers) : result -= evaluate(node.children.at(i), result, identifiers);
            }
            else if (op == "*") {
                i == 0 ? result = evaluate(node.children.at(i), result, identifiers) : result *= evaluate(node.children.at(i), result, identifiers);
            }
            else if (op == "=") {
                if (i == node.children.size() - 1) {
                    //stores the rightmost value, set it to all other identifiers before it, and returns this value
                    result = evaluate(node.children.at(i), result, identifiers);
                    for (size_t j = 0; j < node.children.size() - 1; j++) {
                        identifiers[node.children[j].token.value] = result;
                    }
                }
            }
            else if (op == "/") {
                if (i == 0) {
                    result = evaluate(node.children.at(i), result, identifiers);
                }
                else {
                    if (evaluate(node.children.at(i), result, identifiers) == 0) {
                        std::cout << "Runtime error: division by zero.\n";
                        throw std::runtime_error("");
                    }
                    result /= evaluate(node.children.at(i), result, identifiers);
                }
            }
        }
    }
    else if (node.token.type_ == identifier_) {
        std::string varName = node.token.value;
        if (identifiers.find(varName) != identifiers.end()) {
            return identifiers[varName];
        } 
        else {
             std::cout << "Runtime error: unknown identifier " << varName << "\n";
             throw std::runtime_error("");
         }
    }
    else if (node.token.type_ == number) {
        return std::stod(node.token.value);
    }
    return result;
}

int main() {
    std::string result = "";
    std::istream& input_stream = std::cin;
    char character;

    while (input_stream.get(character)) {
        result += character;
    }

    lex lexer;
    Parser parser;

    try {
        std::vector<Token> tokens = lexer.tokenize(result);
        std::vector<ASTNode> trees;

        while (tokens[parser.index].type_ != end) {
            ASTNode tree = parser.parseExpression(tokens, parser.index, parser.identifiers);
            trees.push_back(tree);
            parser.index++;
        }

        for (auto tree : trees) {
            parser.printInfix(tree);
            std::cout << '\n';
            std::cout << parser.evaluate(tree, parser.result, parser.identifiers) << '\n'; 
        }
    }
    catch (const std::logic_error& e) {
        return 1;
    }
    catch (int e) {
        return 2;
    }
    catch (const std::runtime_error& e) {
        return 3;
    }

    return 0; 
}
