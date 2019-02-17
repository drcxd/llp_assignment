
all:  bin/forth

bin/forth: obj/forth.o obj/util.o
	mkdir -p bin
	ld -o bin/forth obj/forth.o obj/util.o

obj/forth.o: forth.asm macro.inc words.inc
	mkdir -p obj
	nasm -felf64 -o obj/forth.o forth.asm

obj/util.o: util.asm
	mkdir -p obj
	nasm -felf64 -o obj/util.o util.asm

clean:
	rm -rf bin
	rm -rf obj
