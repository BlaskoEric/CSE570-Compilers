FLAGS := -ggdb
CC := gcc

cal: cal.o lex.o
	$(CC) $(FLAGS) cal.o lex.o -lm -o cal

cal.o: cal.tab.c cal.y
	$(CC) $(FLAGS) -c cal.tab.c -o cal.o

lex.o: lex.yy.c
	$(CC) $(FLAGS) -c lex.yy.c -o lex.o

cal.tab.c: cal.y
	bison --debug --verbose -d -k cal.y 

lex.yy.c: cal.l cal.tab.h   
	flex cal.l
 
clean:
	rm -f cal.tab.c lex.yy.c *.o *.cal cal.f core output
