FLAGS = -pedantic -Wall -Werror -std=c11 -g

all : main

main : main.o bmp_trans.o simd.o
	gcc $(FLAGS) -o main main.o bmp_trans.o simd.o -lm


main.o : main.c
	gcc $(FLAGS) -c main.c

bmp_trans.o : bmp_trans.c
	gcc $(FLAGS) -c bmp_trans.c

simd.o : simd.asm
	nasm -felf64 -o simd.o simd.asm

clean :
	rm *.o main
