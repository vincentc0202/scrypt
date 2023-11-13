#include "lib/scrypt.h"


int main () {
    std::string result = "";
    std::istream& input_stream = std::cin;
    char character;


    while (input_stream.get(character)) {
        result += character;
    }

    lex lexer;
    Scrypt scrypt;

    try {
        std::vector<Token> tokens = lexer.tokenize(result);
        //lexer.printTokens(tokens);
        std::reverse(tokens.begin(), tokens.end());
        scrypt.interpret(tokens);
    }
    //lexer error
    catch(const std::logic_error& e){
        return 1;
    } 
    //parse error
    catch(const UnexpToken& e){
        std::cout << e.what() << std::endl;
        return 2;
    } 
    //runtime error
    catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << '\n';
        return 3;
    }


    return 0;
}