link: calc.o
	ld -o calc calc.o

calc.o:
	nasm -f elf64 -o calc.o calc.asm

clean:
	rm *.o
