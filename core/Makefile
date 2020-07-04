ccc: parser.c lexer.c
	cc parser.c lexer.c ast.c -o ccc -lfl

paeser.c: parser.y
	bison -d -o parser.c parser.y

lexer.c: lexer.l
	flex -o lexer.c lexer.l

clean:
	rm ccc parser.c lexer.c