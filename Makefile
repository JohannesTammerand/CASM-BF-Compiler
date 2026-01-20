CC=gcc
CFLAGS=
MAKE = make
PROGRAM = ++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.

.SILENT:
all: bfi asm

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

mem.o: mem.c
	$(CC) $(CFLAGS) -c mem.c

instructions.o: instructions.c
	$(CC) $(CFLAGS) -c instructions.c

bfi: stack.h stack.o mem.h mem.o instructions.h instructions.o
	$(CC) $(CFLAGS) bfi.c mem.o stack.o instructions.o -o bfi

asm: bfi
	./bfi --asm '$(PROGRAM)' > bfiAsm.asm
	nasm bfiAsm.asm -felf
	gcc -m32 -no-pie bfiAsm.o -o bfiAsm

.SILENT:
run: all
	./bfi '$(PROGRAM)'
	echo 3. programm - instructions.c + asm
	./bfiAsm

clean:
	rm -fr bfi stack.o mem.o instructions.o bfiAsm bfi.asm bfiAsm.o bfi.o bfiAsm.asm