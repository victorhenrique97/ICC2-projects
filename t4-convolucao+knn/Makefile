all:
	gcc -o t4 src/*.c -I include/  -lm -g -Wall
run:
	./t4 
debug:
	valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./t4 
