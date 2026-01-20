CC=gcc
CFLAGS=-DDEBUG
all: bfi

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

mem.o: mem.c
	$(CC) $(CFLAGS) -c mem.c

instructions.o: instructions.c
	$(CC) $(CFLAGS) -c instructions.c

bfi: stack.h stack.o mem.h mem.o instructions.h instructions.o
	$(CC) $(CFLAGS) bfi.c mem.o stack.o instructions.o -o bfi	

clean:

	rm -fr bfi stack.o mem.o