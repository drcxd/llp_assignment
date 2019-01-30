
a.out: dict.o lib.so main.o
	ld -o a.out dict.o main.o -d lib.so --dynamic-linker=/lib64/ld-linux-x86-64.so.2

main.o: main.asm words.inc colon.inc
	nasm -felf64 -o main.o main.asm

dict.o: dict.asm
	nasm -felf64 -o dict.o dict.asm

lib.so: lib.o
	ld -shared -o lib.so lib.o

lib.o: lib.asm
	nasm -felf64 -o lib.o lib.asm

clean:
	rm lib.o lib.so dict.o a.out main.o
