#include "lex.h"
#include <iomanip>  //for setw

std::vector<Token> lex::tokenize(std::string input) {
    std::vector<Token> tokens;
    std::vector<char> vect(input.begin(), input.end());
    
    int lineCounter = 1;
    int columnCounter = 1;

    while (vect.size() > 0) {
        if (vect.at(0) == '(') {
            Token token(type::openParen, lineCounter, columnCounter, "(");
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == ')') {
            Token token(type::closeParen, lineCounter, columnCounter, ")");
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == '{') {
            Token token(type::openCurlyBracket, lineCounter, columnCounter, "{");
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == '}') {
            Token token(type::closeCurlyBracket, lineCounter, columnCounter, "}");
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == '[') {
            Token token(type::openBracket, lineCounter, columnCounter, "[");
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == ']') {
            Token token(type::closeBracket, lineCounter, columnCounter, "]");
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == '+' || vect.at(0) == '-' || vect.at(0) == '*' || vect.at(0) == '/' || vect.at(0) == '%') {
            Token token(type::operator_, lineCounter, columnCounter, std::string(1, vect.at(0)));
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == '=') {
            if (vect.size() > 1 && vect.at(1) == '=') {
                Token token(type::equalEqual, lineCounter, columnCounter, "==");
                tokens.push_back(token);
                vect.erase(vect.begin(), vect.begin()+2);
                columnCounter += 2;
            }
            else {
                Token token(type::equal, lineCounter, columnCounter, "=");
                tokens.push_back(token);
                vect.erase(vect.begin());
                columnCounter++;
            }
        } 
        else if (vect.at(0) == '!' && vect.at(1) == '=') {
            Token token(type::notEqual, lineCounter, columnCounter, "!=");
            tokens.push_back(token);
            vect.erase(vect.begin(), vect.begin()+2);
            columnCounter += 2;
        } 
        else if (vect.at(0) == ' ' || vect.at(0) == '\t') {
            vect.erase(vect.begin());
            columnCounter++;
        } 
        else if (vect.at(0) == '\n') {
            vect.erase(vect.begin());
            lineCounter++;
            columnCounter = 1;
        }
        else if (vect.at(0) == '<') {
            if (vect.at(1) == '=') {
                Token token(type::lessThanOrEqual, lineCounter, columnCounter, "<=");
                tokens.push_back(token);
                vect.erase(vect.begin(), vect.begin()+2);
                columnCounter += 2;
            }
            else {
                Token token(type::lessThan, lineCounter, columnCounter, "<");
                tokens.push_back(token);
                vect.erase(vect.begin());
                columnCounter++;
            }
        } 
        else if (vect.at(0) == '>') {
            if (vect.at(1) == '=') {
                Token token(type::greaterThanOrEqual, lineCounter, columnCounter, ">=");
                tokens.push_back(token);
                vect.erase(vect.begin(), vect.begin()+2);
                columnCounter += 2;
            }
            else {
                Token token(type::greaterThan, lineCounter, columnCounter, ">");
                tokens.push_back(token);
                vect.erase(vect.begin());
                columnCounter++;
            }
        } 
        else if (vect.at(0) == '&') {
            Token token(type::logicalAnd, lineCounter, columnCounter, std::string(1, vect.at(0)));
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        } 
        else if (vect.at(0) == '^') {
            Token token(type::logicalXor, lineCounter, columnCounter, std::string(1, vect.at(0)));
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == '|') {
            Token token(type::logicalOr, lineCounter, columnCounter, std::string(1, vect.at(0)));
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        // , and ;
        else if (vect.at(0) == ';') {
            Token token(type::semicolon, lineCounter, columnCounter, std::string(1, vect.at(0)));
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        else if (vect.at(0) == ',') {
            Token token(type::comma, lineCounter, columnCounter, std::string(1, vect.at(0)));
            tokens.push_back(token);
            vect.erase(vect.begin());
            columnCounter++;
        }
        //identifiers, true, and false
        else if (isalpha(vect.at(0)) || vect.at(0) == '_') {
            //can't have number right before letter
            if (tokens.size() > 0 && isdigit(tokens.back().value[0]) && (tokens.back().column == columnCounter - 1)) {
                std::cout << "Syntax error on line " << lineCounter << " column " << columnCounter << "." << std::endl;
                throw std::logic_error("");
                exit(1);
            }

            std::string word = "";
            int currentColumnCounter = columnCounter;
            while (vect.size() > 0 && (isalnum(vect.at(0)) || vect.at(0) == '_')) {
                word += vect.at(0);
                vect.erase(vect.begin());
                columnCounter++;
            }
            if (word == "true") {
                Token token(type::boolTrue, lineCounter, currentColumnCounter, word);
                tokens.push_back(token);
            } 
            else if (word == "false") {
                Token token(type::boolFalse, lineCounter, currentColumnCounter, word);
                tokens.push_back(token);
            } 
            else if (word == "if") {
                Token token(type::ifStatement, lineCounter, currentColumnCounter, word);
                tokens.push_back(token);
            } 
            else if (word == "else") {
                Token token(type::elseStatement, lineCounter, currentColumnCounter, word);
                tokens.push_back(token);
            } 
            else if (word == "while") {
                Token token(type::whileStatement, lineCounter, currentColumnCounter, word);
                tokens.push_back(token);
            } 
            else if (word == "print") {
                Token token(type::printStatement, lineCounter, currentColumnCounter, word);
                tokens.push_back(token);
            } 
            else {
                Token token(type::identifier_, lineCounter, currentColumnCounter, word);
                tokens.push_back(token);
            }
        }

        //for integers and floats
        else if (isdigit(vect.at(0))) {
            std::string number = "";
            int currentColumnCounter = columnCounter;
            int numDecimals = 0;
            while (vect.size() > 0 && (vect.at(0) == '.' || isdigit(vect.at(0)))) {
                //if its a decimal
                if (vect.at(0) == '.') {
                    numDecimals++;
                    //a NUMBER has to be after a decimal
                    if (vect.size() > 1 && !isdigit(vect.at(1))) {
                        columnCounter++;
                        std::string error = "Syntax error on line " + std::to_string(lineCounter) + " column " + std::to_string(columnCounter) + ".\n";
                        std::cout << error;
                        throw std::logic_error(error);
                    }
                    //multiple decimal 
                    else if (numDecimals > 1) {
                        std::cout << "Syntax error on line " << lineCounter << " column " << columnCounter << "." << std::endl;
                        throw std::logic_error("");
                        exit(1);  
                    }
                    //specific trailing decimal edge case --> Ex) 2.         w/ no space after '.'
                    if (vect.size() == 1 && vect.at(0) == '.') {
                        columnCounter++;
                        std::cout << "Syntax error on line " << lineCounter << " column " << columnCounter << "." << std::endl;
                        throw std::logic_error("");
                        exit(1);
                    }
                }
                number += vect.at(0);
                vect.erase(vect.begin());
                columnCounter++;

            }
            Token token(type::number, lineCounter, currentColumnCounter, number);
            tokens.push_back(token);
        }
        //check for unrecognized characters
        else {
            std::cout << "Syntax error on line " << lineCounter << " column " << columnCounter << "." << std::endl;
            throw std::logic_error("");
            exit(1);
        }
        
    }   

    Token endToken(type::end, lineCounter, columnCounter, "END");
    tokens.push_back(endToken);

    return tokens;
}

void lex::printTokens(std::vector<Token> tokens) {
    for (Token token : tokens) { 
        std::cout << std::setw(4) << std::to_string(token.line) << std::setw(5) << std::right << std::to_string(token.column) << "  " << token.value << "\n";
    }
}
