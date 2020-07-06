rm -f ccc parser.c lexer.c parser.h
bison -o parser.c parser.y --defines
flex -o lexer.c lexer.l
cc lexer.c parser.c ast.c -o ccc -lfl