all: main

main: main.o hof.o linked_list.o
	gcc -o main main.o hof.o linked_list.o -g

main.o: main.c
	gcc -c main.c -g

hof.o: hof.c
	gcc -c hof.c -g

linked_list.o: linked_list.c
	gcc -c linked_list.c -g

clean:
	rm main.o hof.o linked_list.o main
