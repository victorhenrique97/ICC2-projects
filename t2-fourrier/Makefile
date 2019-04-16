all:
	gcc -o trab2 src/main.c src/utils.c -I./include -lm -g -Wall

run:
	./trab2

debug:
	valgrind --tool=memcheck --leak-check=yes ./trab2