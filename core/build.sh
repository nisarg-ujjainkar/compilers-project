rm -f ccc parser.cpp lexer.cpp parser.hpp
bison --defines -o parser.cpp parser.y
flex -o lexer.cpp lexer.l
g++ -o ccc parser.cpp lexer.cpp ast.cpp translate.cpp main.cpp -lfl
