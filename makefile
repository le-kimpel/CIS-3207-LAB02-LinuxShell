CFLAGS = -g

objects = main.o linkedlist.o internal.o

myshell : $(objects)
	cc -o myshell $(objects)

main.o : linkedlist.h internal.h

.PHONY : clean
clean :
	rm myshell $(objects)
