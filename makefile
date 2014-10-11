all: compilar
compilar: main.o 
	gcc -o a.out main.o 
main.o: main.c
	gcc -o main.o -c main.c
