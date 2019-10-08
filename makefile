CFLAGS = -g

objects = linkedlist.o new2.o internal.o 

str : $(objects)
	cc -o str $(objects)

new2.o : linkedlist.h internal.h

.PHONY : clean
clean :
	rm str $(objects)
