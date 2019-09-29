CFLAGS = -g

objects = linkedlist.o string.o 

str : $(objects)
	cc -o str $(objects)

string.o : linkedlist.h

.PHONY : clean
clean :
	rm str $(objects)
