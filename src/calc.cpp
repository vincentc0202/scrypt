#include "lib/calc.h"

int main() {
    lex lexer;
    Parser parser;
    std::string input;

    while (std::getline(std::cin, input)) {
        if (input.empty()) {
            break;
        }

        try {
            std::vector<Token> tokens = lexer.tokenize(input);
            // lexer.printTokens(tokens);
            size_t pos = 0;
            parser.parencount = 0;

            std::unique_ptr<ASTNode> root = parser.parseExpression(tokens, pos);
            if (parser.parencount != 0){
                throw UnexpToken("Unexpected token at line " + std::to_string(tokens[pos].line) + " column " + std::to_string(tokens[pos].column)+ ": " + tokens[pos].value);
            }
            // Print the AST in infix form
            root->printInfix(); 
            std::cout << std::endl;

            //stores original values that can be restored in case of runtime error
            auto symbcopy = symbTable;
            
            try {
                // need to include checks for all of these nodes
                if (root->type == "arithmeticOperator" || root->type == "number") {
                    Value result = root->evaluate();
                    std::cout << std::get<double>(result) << '\n';
                }
                else if (root->type == "boolean" || root->type == "boolOperator") {
                    Value result = root->evaluate();
                    std::cout << (std::get<bool>(result) ? "true" : "false") << '\n';
                }
                //assignment nodes and variable nodes can either hold/return a double or boolean value
                else if (root->type == "assignment" || root->type == "variable") {
                    Value result = root->evaluate();
                    if (std::holds_alternative<double>(result)) {
                        std::cout << std::get<double>(result) << '\n';
                    }
                    else {
                        std::cout << (std::get<bool>(result) ? "true" : "false") << '\n';
                    }
                }
            } 
            catch (const std::runtime_error& e) {
                symbTable = symbcopy;
                std::cout << "Runtime error: " << e.what() << '\n';
                continue;
            }
            
        //catch lexer error case
        } 
        catch(const std::logic_error& e){
            continue;
        } 
        catch(const UnexpToken& e){
            std::cout << e.what() << std::endl;
        
        } 
        //for "Runtime error:" printing
        catch (const std::exception& e) {
            if (typeid(e) == typeid(std::logic_error)) {
                std::cout << "Logic errror: " << e.what() << '\n';
            } 
            else {
                std::cout << "Runtime error: " << e.what() << '\n';
            }
        } 
    }
    return 0;
}

