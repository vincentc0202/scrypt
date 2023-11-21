CXXFLAGS = -g -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter 
CXX      = g++

# These targets don't create any files:
.PHONY: scrypts clean

# Magic Variables!
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#  $@ is the name of the target
#  $+ is a list of all the dependencies
#  $< is the first dependency


scrypts: lex calc format scrypt

lex: lex.o lexer.o
	${CXX} $(CXXFLAGS) -o $@ $+

calc: calc.o calculator.o lexer.o scrypter.o
	${CXX} $(CXXFLAGS) -o $@ $+
  
format: format.o formatter.o lexer.o calculator.o  scrypter.o
	${CXX} $(CXXFLAGS) -o $@ $+

scrypt: scrypt.o scrypter.o lexer.o calculator.o 
	${CXX} $(CXXFLAGS) -o $@ $+



lex.o: lex.cpp
	${CXX} $(CXXFLAGS) -c -o $@ $<

lexer.o: lib/lex.cpp
	${CXX} $(CXXFLAGS) -c -o $@ $<

calc.o: calc.cpp
	${CXX} $(CXXFLAGS) -c -o $@ $<

calculator.o: lib/calc.cpp
	${CXX} $(CXXFLAGS) -c -o $@ $<

format.o: format.cpp
	${CXX} $(CXXFLAGS) -c -o $@ $<

formatter.o: lib/format.cpp
	${CXX} $(CXXFLAGS) -c -o $@ $<

scrypt.o: scrypt.cpp
	${CXX} $(CXXFLAGS) -c -o $@ $<
	
scrypter.o: lib/scrypt.cpp
	${CXX} $(CXXFLAGS) -c -o $@ $<


clean:
	rm *.o lex calc format scrypt

