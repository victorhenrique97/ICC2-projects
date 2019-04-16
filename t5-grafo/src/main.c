#include <stdlib.h>
#include <stdio.h>
#include <maze_runner.h>
#include <utils.h>

int main(int argc, char *argv[]){
	
	int vertex_number = 0, chamber_number = 0, segment_number = 0, vertex_start = 0;
	VERTEX **vertex = NULL;
	SEGMENT **segment = NULL;

	scanf("%d%*[ \n]", &vertex_number);
	vertex_number++; 						// estou incrementando uma vez que o primeiro vertice está na posição 1
	vertex = setvertex(vertex_number);

	scanf("%d%*[ \n]", &chamber_number);
	setchamber(vertex, chamber_number);

	scanf("%d%*[ \n]", &segment_number);
	segment = setsegment(segment_number);

	scanf("%d%*[ \n]", &vertex_start);

	produce_solution(vertex, vertex_number, segment, segment_number, vertex_start);

	free_vertex(vertex, vertex_number);
	free_segment(segment, segment_number);

return 0;
}