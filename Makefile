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

asm: bfi
	./bfi '++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.' > bfiAsm.asm
	nasm bfiAsm.asm -felf
	gcc -m32 -no-pie bfiAsm.o -o bfiAsm
	./bfiAsm

clean:
	rm -fr bfi stack.o mem.o bfiAsm bfi.asm bfiAsm.o bfi.o bfiAsm.asm