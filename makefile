CFLAGS = -g

objects = linkedlist.o new.o 

str : $(objects)
	cc -o str $(objects)

new.o : linkedlist.h

.PHONY : clean
clean :
	rm str $(objects)
