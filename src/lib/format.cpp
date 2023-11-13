#include "format.h"


void printIndents(int counter) {
    if (counter == 0) {
        return;
    }
    for (int i = 0; i < counter; i++) {
        std::cout << "    ";
    }
}

void Format::printFormat(std::vector<Token>& tokens) {
    //NOTE: the tokens are already reversed because it makes popping from back is much more efficient than popping from front
    Parser parser;
    int curlyCounter = 0;
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

            if (tokens.back().type_ != semicolon) {
                throw UnexpToken("Unexpected token at line " + std::to_string(tokens.back().line) + " column " + std::to_string(tokens.back().column) + ": " + tokens.back().value);
            }
            
            tokens.pop_back();
            std::unique_ptr<ASTNode> root = parser.parseExpression(tempTokens, pos);
            printIndents(curlyCounter);
            root->printInfix();
            std::cout << ";\n";

        }
    }
}