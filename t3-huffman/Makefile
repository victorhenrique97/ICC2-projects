all:
	gcc src/*.c -I include/ -o main -lm -g -Wall
run:
	./main
fullrun:
	valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./main