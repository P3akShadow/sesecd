all: compiler 

compiler: parser.tab.c lex.yy.c secd.c
	gcc parser.tab.c lex.yy.c secd.c -o compiler

parser.tab.c: parser.y
	bison -d parser.y

lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm parser.tab* lex.yy*
