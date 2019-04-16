all:
	gcc -o t5 src/*.c -I include/  -lm -g -Wall
run:
	./t5
debug:
	valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./t5
