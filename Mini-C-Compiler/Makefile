all: compiler run

compiler: lexical.l parser.y
	flex lexical.l
	bison -d parser.y
	gcc lex.yy.c parser.tab.c -o compiler -lfl

run: compiler
	./compiler input.min > output.txt
	cat output.txt

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h compiler output.txt
