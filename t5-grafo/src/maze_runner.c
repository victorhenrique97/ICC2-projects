#include <stdlib.h>
#include <stdio.h>
#include <maze_runner.h>
#include <utils.h>

///////////////////////////////////////////////////////////////////////////////
int check_track(int *trackA, int *trackB, int len){

	for(int i=0; i<len; i++){ 			// estou checando se a camara de A é maior que a de B, se for devo ordenar
		if(trackA[i] > trackB[i]) return 1;
		else if(trackA[i] < trackB[i]) return 0;
	}

return 0;	
}
///////////////////////////////////////////////////////////////////////////////
/*Estou ordenando as saidas de acordo com o que foi pedido no PDF*/
void order_solution(SOLUTION **solution, int counter){

	SOLUTION *solution_aux;

	for(int i=counter-1; i>0; i--)
		for(int j=0; j<i; j++)
			if((solution[j]->distance > solution[j+1]->distance)|| 
				(solution[j]->distance == solution[j+1]->distance && 
					solution[j]->vertex_passed_numbers > solution[j+1]->vertex_passed_numbers) ||
				(solution[j]->distance == solution[j+1]->distance && 
					solution[j]->vertex_passed_numbers == solution[j+1]->vertex_passed_numbers &&
					check_track(solution[j]->track, solution[j+1]->track, solution[j]->vertex_passed_numbers))){
				solution_aux = solution[j];
				solution[j] = solution[j+1];
				solution[j+1] = solution_aux;
			}
}
///////////////////////////////////////////////////////////////////////////////
/*Pego da pilha os indices das soluções, coloco em um vetor, inverto ele uma vez que leio do topo para base
depois calculo a distancia euclidiana*/
SOLUTION *get_solution(STACK *stack, VERTEX **vertex){

	SOLUTION *solution = (SOLUTION *) calloc(1, sizeof(SOLUTION));
	ELEM *elem = stack->end;
	int counter = 0;

	while(elem != NULL){
		solution->track = (int *) realloc(solution->track, sizeof(int)*(counter+1));
		solution->track[counter++] = elem->vertex_index;
		elem = elem->down;
	}
	solution->vertex_passed_numbers = counter;
	vector_inversor(solution->track, solution->vertex_passed_numbers);

	for(int i=0, j=1; j<counter; i++, j++)
		solution->distance += euclidean_distance(vertex[solution->track[i]]->x, vertex[solution->track[i]]->y,
			vertex[solution->track[j]]->x, vertex[solution->track[j]]->y);

return solution;
}

///////////////////////////////////////////////////////////////////////////////
void produce_solution(VERTEX **vertex, int vertex_number, SEGMENT **segment, int segment_number, int vertex_start){

	STACK *stack = NULL;
	SOLUTION **solution = NULL;
	int i=0, counter = 0;
	push(&stack, vertex_start, segment_number);

	while(stack->end){
		if(vertex[stack->end->vertex_index]->exit == TRUE && 	// se eu não estiver passsado por essa camara nesse caminho, e ela tiver saida eu pego o caminho
			vertex[stack->end->vertex_index]->passed == FALSE){
			solution = (SOLUTION **) realloc(solution, sizeof(SOLUTION *)*(counter+1));
			solution[counter++] = get_solution(stack, vertex);
		}

		vertex[stack->end->vertex_index]->passed = TRUE; // seto uma flag que indica que passei por aqui

		for(i=0; i<segment_number; i++){
			if(segment[i]->isvalid == TRUE && stack->end->passed[i] != TRUE){ // uso um vetor para garantir que não vou para um segmento que ja passei antes

				if(segment[i]->begin_index == stack->end->vertex_index &&
					vertex[segment[i]->end_index]->passed == FALSE){
					stack->end->passed[i] = TRUE;
					push(&stack, segment[i]->end_index, segment_number); // empilho o indice do vertice
					break;
				}

				else if(segment[i]->end_index == stack->end->vertex_index &&
					vertex[segment[i]->begin_index]->passed == FALSE){
					stack->end->passed[i] = TRUE;
					push(&stack, segment[i]->begin_index, segment_number);
					break;
				}
			}
		}
		if(i == segment_number){ // caso não haja mais opçoes, desempilho e seto o caminho atual para false
			vertex[stack->end->vertex_index]->passed = FALSE;
			pop(stack);
		}
	}
	
	free(stack);
	order_solution(solution, counter);

	for(int i=0; i<counter; i++){
		printf("%d ", solution[i]->vertex_passed_numbers);
		for(int j=0; j<solution[i]->vertex_passed_numbers; j++)
			printf("%d ", solution[i]->track[j]);
		printf("%d\n", (int)solution[i]->distance);
	}

	free_solution(solution, counter);
}

///////////////////////////////////////////////////////////////////////////////
/*Leio da entrada, caso o segmento seja maior que zero, é valido*/
SEGMENT **setsegment(int segment_number){

	SEGMENT **segment = allocsegment(segment_number);

	for(int i=0; i<segment_number; i++){
		scanf("%d%*[ \n]%d%*[ \n]", &segment[i]->begin_index, &segment[i]->end_index);
		if(segment[i]->begin_index > 0 && segment[i]->end_index > 0)segment[i]->isvalid = TRUE;
		else segment[i]->isvalid = FALSE;
	}

return segment;
}

//////////////////////////////////////////////////////////////////////////////
/*Seto uma flag que indica que há uma câmara neste vertice, e se há saida*/
void setchamber(VERTEX **vertex, int chamber_number){

	int vertex_index;
	int exit;

	for(int i=0; i<chamber_number; i++){
		scanf("%d%*[ \n]", &vertex_index);
		vertex[vertex_index]->chamber = TRUE;
		scanf("%d%*[ \n]", &exit);
		if(exit) vertex[vertex_index]->exit = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
/*Leio da entrada e armazeno as coordenadas de cada vértice*/
VERTEX **setvertex(int vertex_number){

	VERTEX **vertex = allocvertex(vertex_number);

	for(int i=1; i<vertex_number; i++){
		scanf("%f%*[ \n]%f%*[ \n]", &vertex[i]->x , &vertex[i]->y);
		vertex[i]->chamber = FALSE;
		vertex[i]->exit = FALSE;
		vertex[i]->passed = FALSE;
	}

return vertex;
}