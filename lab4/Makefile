closure: closure.o LR0Item.o
	g++ -o closure closure.o LR0Item.o

LR0Item.o: LR0Item.cpp LR0Item.h
	g++ -c LR0Item.cpp 

closure.o: closure.cpp 
	g++ -c closure.cpp
clear:
	rm *.o
