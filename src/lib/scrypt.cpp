#include "scrypt.h"

Parser parser;

void Scrypt::deleteBlock(std::vector<Token>& tokens) {
    //deletes openCurly and every expression until it reaches its corresponding end curly and deletes that
    tokens.pop_back();

    int curlyCounter = 1;
    while (tokens.size() > 0 && curlyCounter != 0) {
        if (tokens.back().type_ == closeCurlyBracket)
            curlyCounter--;
        else if (tokens.back().type_ == openCurlyBracket)
            curlyCounter++;
        tokens.pop_back();
    }
}

std::vector<Token> Scrypt::getExpression(std::vector<Token>& tokens) {
    //processes boolExpression for statements. does NOT process curlybraces
    std::vector<Token> tempTokens;
    while (tokens.size() > 0 && (tokens.back().type_ != openCurlyBracket))   {
        tempTokens.push_back(tokens.back());
        tokens.pop_back();
    }
    return tempTokens;
}

std::unique_ptr<ASTNode> Scrypt::evalExpression(std::vector<Token>& tokens) {
    //processes boolExpression for statements. does NOT process curlybraces
    size_t pos = 0;
    std::unique_ptr<ASTNode> root = parser.parseExpression(tokens, pos);

    if (root->type == "boolOperator" || root->type == "boolean" || (root->type == "variable" && std::holds_alternative<bool>(root->evaluate()))) {}
    else
        throw std::runtime_error("condition is not a bool.");
    
    return root;
}

void Scrypt::interpret(std::vector<Token>& tokens) {

    while (tokens.size() > 1) { 
        std::vector<Token> tempTokens;
        //std::vector<std::shared_ptr<FunctionNode>> functionList;
        size_t pos = 0;

        if (tokens.back().type_ == ifStatement) {
            bool prevCondition = true;

            tokens.pop_back();

            tempTokens = getExpression(tokens);
            std::unique_ptr<ASTNode> root = evalExpression(tempTokens);
            Value result = root->evaluate();

            //if the boolean expression is true
            if (std::get<bool>(result)) {
                //delete open curly, process block, delete close curly
                tokens.pop_back();
                while (tokens.back().type_ != closeCurlyBracket) {
                    interpret(tokens);
                }
                tokens.pop_back();

                //ignore the else AND else ifs, if they exist
                while (tokens.back().type_ == elseStatement) {
                    while (tokens.back().type_ != openCurlyBracket)
                        tokens.pop_back();
                    deleteBlock(tokens);
                }

                //check if it has been recursively called
                if (tokens.back().type_ == closeCurlyBracket) return;
            }
            //if the boolean expression is false
            else if (!std::get<bool>(result)) {
                prevCondition = false;
                deleteBlock(tokens);

                //process elses, if there they exist
                while (tokens.back().type_ == elseStatement) {
                    //delete the else first
                    tokens.pop_back();

                    //else if
                    if (tokens.size() > 0 && tokens.back().type_ == ifStatement) {
                        tokens.pop_back();
                        
                        tempTokens = getExpression(tokens);
                        std::unique_ptr<ASTNode> root = evalExpression(tempTokens);
                        Value result = root->evaluate();
                        //on open curly right now

                        if (std::get<bool>(result)) {
                            prevCondition = true;
                            tokens.pop_back();
                            while (tokens.back().type_ != closeCurlyBracket) {
                                interpret(tokens);
                            }
                            tokens.pop_back();
                        }
                        else if (!std::get<bool>(result)) {
                            deleteBlock(tokens);
                        }
                    }
                    //else
                    else if (tokens.size() > 0 && tokens.back().type_ == openCurlyBracket) {
                        if (prevCondition) {
                            deleteBlock(tokens);
                        }
                        else {
                            tokens.pop_back();
                            while (tokens.back().type_ != closeCurlyBracket) {
                                interpret(tokens);
                            }
                            tokens.pop_back();
                        }
                    }
                }

                //check if it has been recursively called
                if (tokens.back().type_ == closeCurlyBracket) return;
            }

        }
        else if (tokens.back().type_ == whileStatement) {
            tokens.pop_back();


            tempTokens = getExpression(tokens);
            std::unique_ptr<ASTNode> root = evalExpression(tempTokens);
            Value result = root->evaluate();
            tokens.pop_back();

            std::vector<Token> braceBlock = tokens;
            //process braceBlock
            while (std::get<bool>(result)) {
                tokens = braceBlock;
                interpret(tokens);


                root = evalExpression(tempTokens);
                result = root->evaluate();
            }

            //delete closeCurly
            tokens.pop_back();

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
            
            //pop the semicolon
            tokens.pop_back();

            std::unique_ptr<ASTNode> root = parser.parseExpression(tempTokens, pos);
            Value result = root->evaluate();

            //printing syntax
            if (std::holds_alternative<double>(result)) {
                std::cout << std::get<double>(result) << '\n';
            }
            else {
                std::cout << (std::get<bool>(result) ? "true" : "false") << '\n';
            }

            if (tokens.size() > 0 && tokens.back().type_ == closeCurlyBracket) return;
        }
        //funcdef
        else if (tokens.back().type_ == functionDefinitionStatement){
            std::string funcName;
            std::vector<std::unique_ptr<VariableNode>> parameters;
            std::vector<Token> block;
            //delete "def"
            tokens.pop_back();

            //process funcName;
            funcName = tokens.back().value;
            tokens.pop_back();

            //process parameters
            tokens.pop_back();
            while (tokens.back().type_ != closeParen) {
                std::unique_ptr<VariableNode> param = std::make_unique<VariableNode>(tokens.back().value);
                parameters.push_back(std::move(param));
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
            }

            auto param = std::make_shared<FunctionNode>(funcName, std::move(parameters), block);
            functionDef[funcName] = std::move(param);          
        }

        
       
        else if(tokens.back().type_ == identifier_ ){
            if(tokens.size() > 1 && tokens[tokens.size() - 2].type_ == openParen){
                
                std::string funcName = tokens.back().value;
                auto name = functionDef.find(funcName);
                if (name != functionDef.end()) {
                    auto& funcNode = name->second;
                    // Copy the statement block of the function
                    std::vector<Token> statementBlockCopy = funcNode->statementBlock;

                    // Execute the statement block
                    interpret(statementBlockCopy);

                    
                } else {
                // Function not found
                throw std::runtime_error("Function " + funcName + " not defined.");
                }
            }
        }

        else if (tokens.back().type_ == returnStatement){
            std::unique_ptr<ASTNode> returnExpression;
        }
        //else if(token)

        else {  // if the statement is just an expression
            int currentLineCounter = tokens.back().line;

            while (tokens.size() > 0 && tokens.back().line == currentLineCounter && (tokens.back().type_ != semicolon && tokens.back().type_ != closeCurlyBracket)) {
                tempTokens.push_back(tokens.back());
                tokens.pop_back();
            }  

            if (tokens.back().type_ != semicolon) {
                throw UnexpToken("Unexpected token at line " + std::to_string(tokens.back().line) + " column " + std::to_string(tokens.back().column) + ": " + tokens.back().value);
            }
            
            //pop the semicolon
            tokens.pop_back();

            std::unique_ptr<ASTNode> root = parser.parseExpression(tempTokens, pos);
            //evaluate() puts it in the map AFTER filtering out any errors
            root->evaluate();  

            //if at the end of braced block, return as it must have been recursively called
            if (tokens.back().type_ == closeCurlyBracket) return;
        }
    }
}

