#include "format.h"
#include "calc.h"

void Format::printIndents(int counter) {
    if (counter == 0) {
        return;
    }
    for (int i = 0; i < counter; i++) {
        std::cout << "    ";
    }
}

void Format::printFormat(std::vector<Token>& tokens, int curlyCounter) {
    //NOTE: the tokens are already reversed because popping from back is much more efficient than popping from front
    Parser parser;
    // int curlyCounter = 0;
    bool elifCurly = false;
    int elifCurlyCounter = 0;

    while (tokens.size() > 0) {
        std::vector<Token> tempTokens;
        size_t pos = 0;

        if (tokens.back().type_ == ifStatement) {
            printIndents(curlyCounter);
            std::cout << "if ";
            tokens.pop_back();

            while (tokens.size() > 0 && (tokens.back().type_ != openCurlyBracket))   {
                tempTokens.push_back(tokens.back());
                tokens.pop_back();
            }

            std::unique_ptr<ASTNode> root = parser.parseExpression(tempTokens, pos);
            root->printInfix();
        }
        else if (tokens.back().type_ == elseStatement) {
            printIndents(curlyCounter);
            std::cout << "else"; 
            tokens.pop_back();

            //BRAINSTORM ABOUT ELSE IF
            if (tokens.size() > 0 && tokens.back().type_ == openCurlyBracket) {
                continue;
            }
            else if (tokens.size() > 0 && tokens.back().type_ == ifStatement) {
                std::cout << " {\n";
                curlyCounter++;
                elifCurly = true;
                elifCurlyCounter = curlyCounter;
                //std::cout << elifCurlyCounter <<'\n';
            }
        }
        else if (tokens.back().type_ == whileStatement) {
            printIndents(curlyCounter);
            std::cout << "while "; 
            tokens.pop_back();

            while (tokens.size() > 0 && (tokens.back().type_ != openCurlyBracket)) {
                tempTokens.push_back(tokens.back());
                tokens.pop_back();
            }

            std::unique_ptr<ASTNode> root = parser.parseExpression(tempTokens, pos);
            root->printInfix();
        }
        else if (tokens.back().type_ == printStatement) {
            int currentLineCounter = tokens.back().line;
            tokens.pop_back();

            while (tokens.back().line == currentLineCounter && tokens.back().type_ != semicolon) {
                tempTokens.push_back(tokens.back());
                tokens.pop_back();
            }   

            if (tokens.back().type_ != semicolon) {
                throw UnexpToken("Unexpected token at line " + std::to_string(tokens.back().line) + " column " + std::to_string(tokens.back().column) + ": " + tokens.back().value);
            }
            
            tokens.pop_back();
            printIndents(curlyCounter);
            std::cout << "print "; 

            std::unique_ptr<ASTNode> root = parser.parseExpression(tempTokens, pos);
            root->printInfix();
            std::cout << ";\n";
        }
        else if (tokens.back().type_ == functionDefinitionStatement) {
            printIndents(curlyCounter);
            std::cout << "def "; 
            tokens.pop_back();

            std::string funcName;
            std::vector<Token> parameters;
            std::vector<Token> block;

            //process funcName; (only processes if the function identifier starts with a letter)
            if (tokens.back().type_ == identifier_) {
                funcName = tokens.back().value;
                tokens.pop_back();
            }
            else {
                throw std::runtime_error("not a function.");
            }

            //process parameters
            tokens.pop_back();
            while (tokens.back().type_ != closeParen) {
                parameters.push_back(tokens.back());
                tokens.pop_back();

                if (tokens.back().type_ == comma) {
                    tokens.pop_back();
                }
            }
            tokens.pop_back();

            //process block
            if (tokens.back().type_ == openCurlyBracket) {
                tokens.pop_back();

                int curlyCounter = 1;
                while (tokens.size() > 0 && curlyCounter != 0) {
                    if (tokens.back().type_ == closeCurlyBracket)
                        curlyCounter--;
                    else if (tokens.back().type_ == openCurlyBracket)
                        curlyCounter++;
                    block.push_back(tokens.back());
                    tokens.pop_back();
                }   
                //get rid of last }
                block.pop_back();
            }   

            std::unique_ptr<FunctionDefNode> functionDef = std::make_unique<FunctionDefNode>(funcName, parameters, block);
            FunctionPtr function = std::make_shared<Function>(functionDef.get(), parameters, block);
            symbTable[funcName] = function;
            functionDef->printInfix(curlyCounter);
        }
        else if (tokens.back().type_ == returnStatement) {
            int currentLineCounter = tokens.back().line;
            tokens.pop_back();
            printIndents(curlyCounter);

            while (tokens.back().line == currentLineCounter && tokens.back().type_ != semicolon) {
                tempTokens.push_back(tokens.back());
                tokens.pop_back();
            }   
            //delete ;
            tokens.pop_back();
            
            std::unique_ptr<ASTNode> root;

            size_t pos2 = 0;
            if (tempTokens.size() == 0) {
                root = nullptr;
            }
            else {
                root = parser.parseExpression(tempTokens, pos2);
            }

            std::unique_ptr<ReturnNode> returnNode = std::make_unique<ReturnNode>(std::move(root));
            returnNode->printInfix();
        }
        else if (tokens.back().type_ == openCurlyBracket) {
            std::cout << " {\n";
            curlyCounter++;
            tokens.pop_back();
        }
        else if (tokens.back().type_ == closeCurlyBracket) {
            printIndents(--curlyCounter);
            std::cout << "}\n";
            tokens.pop_back();

            //conditions needed to detect the elif's closing curly parenthesis
            if (elifCurly && (elifCurlyCounter == curlyCounter) && (tokens.back().type_ == end || tokens.back().type_ == closeCurlyBracket)) {
                printIndents(--curlyCounter);
                std::cout << "}\n";
            }

        }
        else if (tokens.back().type_ == end) {
            break;
        }
        else {  // if the statement is just an expression
            int currentLineCounter = tokens.back().line;

            while (tokens.size() > 0 && tokens.back().line == currentLineCounter && tokens.back().type_ != semicolon) {
                tempTokens.push_back(tokens.back());
                tokens.pop_back();
            }   

            std::unique_ptr<ASTNode> root = parser.parseExpression(tempTokens, pos);

            if (tokens.back().type_ != semicolon) {
                throw UnexpToken("Unexpected token at line " + std::to_string(tokens.back().line) + " column " + std::to_string(tokens.back().column) + ": " + tokens.back().value);
            }
            
            tokens.pop_back();
            //puts variables in the map
            // Value value = root->evaluate();
            printIndents(curlyCounter);
            root->printInfix();
            std::cout << ";\n";
        }
    }
}