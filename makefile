CFLAGS = -g

objects = linkedlist.o main.o internal.o 

str : $(objects)
	cc -o str $(objects)

main.o : linkedlist.h internal.h

.PHONY : clean
clean :
	rm str $(objects)
