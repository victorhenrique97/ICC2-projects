#include <stdlib.h>
#include <stdio.h>
#include <maze_runner.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
void free_segment(SEGMENT **segment, int segment_number){

	for(int i=0; i<segment_number; i++)
		free(segment[i]);
	free(segment);
}
///////////////////////////////////////////////////////////////////////////////
void free_vertex(VERTEX **vertex, int vertex_number){

	for(int i=0; i<vertex_number; i++)
		free(vertex[i]);
	free(vertex);	
}

///////////////////////////////////////////////////////////////////////////////
void free_solution(SOLUTION **solution, int counter){

	for(int i=0; i<counter; i++){
		free(solution[i]->track);
		free(solution[i]);
	}
	free(solution);

}
///////////////////////////////////////////////////////////////////////////////
void vector_inversor(int *vector, int nelements){

	int aux;

	for(int i=0, j=nelements-1; i<j; i++, j--){
		aux = vector[i];
		vector[i] = vector[j];
		vector[j] = aux;
	}
}
///////////////////////////////////////////////////////////////////////////////
float euclidean_distance(float x1, float y1, float x2, float y2){

	float dist = sqrt(pow((x1-x2),2)+pow((y1-y2),2));

return dist;
}

///////////////////////////////////////////////////////////////////////////////
void pop(STACK *stack){

	ELEM *aux = stack->end;
	stack->end = aux->down;
	free(aux->passed);
	free(aux);
}
//////////////////////////////////////////////////////////////////////////////
void push(STACK **stack, int vertex_index, int segment_number){

	if(!(*stack)){
		*stack =(STACK *)calloc(1, sizeof(STACK));
		ELEM *elem = (ELEM *)calloc(1, sizeof(ELEM));
		elem->passed = (BOOL *) calloc(segment_number, sizeof(BOOL));
		elem->vertex_index = vertex_index;
		(*stack)->end = (*stack)->start = elem;
	}
	else{
		ELEM *elem = (ELEM *)calloc(1, sizeof(ELEM));
		elem->passed = (BOOL *) calloc(segment_number, sizeof(BOOL));
		elem->vertex_index = vertex_index;
		elem->down = (*stack)->end;
		(*stack)->end = elem; 
	}
}

///////////////////////////////////////////////////////////////////////////////
SEGMENT **allocsegment(int segment_number){

	SEGMENT **segment = (SEGMENT **)malloc(segment_number * sizeof(SEGMENT *));

	for(int i=0; i<segment_number; i++)
		segment[i] = (SEGMENT *) malloc(sizeof(SEGMENT));

return segment;
}
//////////////////////////////////////////////////////////////////////////////
VERTEX **allocvertex(int vertex_number){

	VERTEX **vertex = (VERTEX **) malloc(vertex_number*sizeof(VERTEX *));
	
	for(int i=0; i<vertex_number; i++)
		vertex[i] =(VERTEX *) malloc(sizeof(VERTEX));

return vertex;
}
