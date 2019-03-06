FLAGS = -pedantic -Wall -Werror -std=c11

all : img_rot

img_rot : image_rot.o bmp_trans.o
	gcc $(FLAGS) -o img_rot image_rot.o bmp_trans.o

image_rot.o :
	gcc $(FLAGS) -c image_rot.c

bmp_trans.o : bmp_trans.c
	gcc $(FLAGS) -c bmp_trans.c

clean :
	rm *.o img_rot
