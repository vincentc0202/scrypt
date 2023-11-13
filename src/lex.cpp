#include "lib/lex.h" 

int main() {
    std::string result = "";
    std::istream& input_stream = std::cin;
    char character;

    while (input_stream.get(character)) {
        result += character;
    }

    try {
        lex lexer;
        std::vector<Token> tokens = lexer.tokenize(result);
        lexer.printTokens(tokens);
    }
    catch (const std::logic_error& e) {
        return 1;
    }

    return 0;
}