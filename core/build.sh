rm -f ccc parser.cpp lexer.cpp parser.hpp
bison --defines -o parser.cpp parser.y
flex -o lexer.cpp lexer.l
#g++ -o ccc ast.cpp lexer.cpp parser.cpp -lfl
# echo "Compiling ast"
# g++ -c ast.cpp -o ast.o
# echo "compiling lexer"
# g++ -c lexer.cpp -o lexer.o -lfl
# echo "compiling parser"
# g++ -c parser.cpp -o parser.o
# echo "final"
# g++ ast.o lexer.cpp parser.cpp -o ccc -lfl
clang++ -o ccc parser.cpp lexer.cpp ast.cpp -lfl
