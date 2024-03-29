#include "lib/format.h"
#include "lib/calc.h"

int main () {
    std::string result = "";
    std::istream& input_stream = std::cin;
    char character;

    while (input_stream.get(character)) {
        result += character;
    }

    lex lexer;
    Format format;

    try {
        std::vector<Token> tokens = lexer.tokenize(result);
        //lexer.printTokens(tokens);
        std::reverse(tokens.begin(), tokens.end());
        format.printFormat(tokens);
    }
    //lexer error
    catch(const std::logic_error& e){
        return 1;
    } 
    //parse error?
    catch(const UnexpToken& e){
        std::cout << e.what() << std::endl;
        return 2;
    } 
    

    return 0;
}