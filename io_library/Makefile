a.out: test.o
	ld test.o -o a.out

test.o: test.asm
	nasm -felf64 test.asm -o test.o

clean:
	rm test.o a.out
