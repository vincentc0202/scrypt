# Scrypt : Part 4 of 4 (CS 32 Fall 2023)

## What the project does, and why: 
This is Part 1 of 4 of a project to build our own programming language. **_(10/19/23)_**  
Part 1 is a calculator that is built up two main parts: a "lexer" and a "parser". 
A lexer basically turns raw, meaningless text into a sequence of tokens that assign value to our text so that the computer knows how evaluate it. 
A parser takes the sequence of tokens from the lexer and builds an abstract syntax tree to represent the calculation described by the input. 

This is Part 2 of 4 of the scrypt project. **_(10/29/23)_**  
In Track A, we modified our existing lexer to include assignment and identifiers, and modified the existing parser to parse S-expressions that contain the new token types mentioned above.
In Track B, we created an entirely new parser that parses human-friendly infix expressions while supporting operations such as parenthesis, multipleication, division, addition, subtraction, and assignment to identifiers. 

This is Part 3 of 4 of the scrypt project. **_(11/6/23)_**  
In this checkpoint, we added in even more operators into our calculator. We added ordered comparisons (<, <=, >, >=), equality & inequality (==, !=), and the logical and, logical XOR, and logical inclusive or operators (&, ^, |). With the addition of these operators came a new data type that is now supported, booleans! In track B, we added 3 new statements, `if, while, print` to add control flow, looping, and printing to our calculator. 

This is Part 4 of 4 of the scrypt project. **_(11/12/23)_**  
In this final checkpoint, we were able to implement functions fully in both format and scrypt, however we were unable to complete resizable arrays as features of our language. In order to do this, we updated our Value variant to include new types, such as a function, null, and arrays. We also created 3 new nodes in our `calc.h` file to represent a function definition, a function call, and a return statement. Overall this project was a great learning experience, and we wish we could have implemented arrays, however we ran out of time on the last track. 

## How to build the project:
In our Github, we have a Makefile located within the `src` library. In order to build the project, simply run `make` in the terminal (double check that you're inside of the `src` folder). The Makefile creates 3 object files from `lib/calc.cpp, lib/lex.cpp, lib/format.cpp, and lib/scrypt.cpp` and links them together into an executable.   

In order to clean up all of the `*.o` files and the `lex`, `calc`, `format`, and `scrypt` executables after running the `make` command, run `make clean` to get rid of the extraneous files. 

## How to use the executables once they're built:
The executables that are created from the makefile are called `lex`, `calc`, `format`, and `scrypt`.  
In order to run these executable, go to the terminal and run the command `./lex` to run the lexer portion of our program, `./calc` to run the infix expression parser, `./format` to run the portion of our program that prints out the user input, and `./scrypt` to run the portion of our program that reads user input, builds an AST of the program, and evaluates the tree.  

Once this command is run, the program will be waiting for user input in the form of a valid sequence of tokens for the lexer, valid infix expressions for calc, and whole blocks of statements in format and scrypt. If the input is invalid, then, an error will be thrown. 

## An overview of how the code is organized:
Within this repository, our code is organized into one main `src` directory which contains all of the source code. Within `src`, we have all of our `.cpp` files that contain only main() functions. There is also a sub-directory `src/lib` which contain `.h` files and `.cpp` files that hold all of the implementations of the lexer, calc, format, and scrypt files. Separating the implementation of our `.cpp` files from their main() functions allows our code to be more modular, as we can easily utilize other files within our "library" by including them at the top of new files. 