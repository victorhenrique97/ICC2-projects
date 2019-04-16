all:
	gcc -o t6 src/*.c -I include/  -lm -g -Wall
run:
	./t6
debug:
	valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./t6
