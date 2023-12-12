.SUFFIXES:
prueba: main.o y.tab.o
		gcc -o prueba main.o y.tab.o
y.tab.o: y.tab.c
		gcc -c y.tab.c
main.o: main.c
		gcc -c main.c
y.tab.c: prueba.y lex.yy.c
		bison -v -o y.tab.c prueba.y
lex.yy.c: prueba.l
		flex prueba.l
clean:
		rm -rf prueba main.o y.tab.o y.tab.c lex.yy.c
todo:
		make clean
		make prueba