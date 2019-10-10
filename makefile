CFLAGS = -g

objects = linkedlist.o process.o internal.o 

str : $(objects)
	cc -o str $(objects)

process.o : linkedlist.h internal.h

.PHONY : clean
clean :
	rm str $(objects)
