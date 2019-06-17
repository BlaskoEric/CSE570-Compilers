actionGoto: actionGoto.o LR0Item.o
	g++ -o actionGoto actionGoto.o LR0Item.o
LR0Item.o: LR0Item.cpp LR0Item.h
	g++ -c LR0Item.cpp 
actionGoto.o: actionGoto.cpp
	g++ -c actionGoto.cpp
clear:
	rm actionGoto *.o
