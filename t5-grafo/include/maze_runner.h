#ifndef _MAZE_RUNNER_H_
#define _MAZE_RUNNER_H_

typedef unsigned char BOOL;
typedef struct ELEM ELEM;
typedef struct STACK STACK;

typedef struct{
	float x;
	float y;
	BOOL chamber;
	BOOL exit;
	BOOL passed;
}VERTEX;

typedef struct{
	int begin_index;
	int end_index;
	BOOL isvalid;
}SEGMENT;

struct ELEM{
	int vertex_index;
	ELEM *down;
	BOOL *passed;
};

struct STACK{
	ELEM *start;
	ELEM *end;
};

typedef struct{
	int *track;
	int vertex_passed_numbers;
	float distance;
}SOLUTION;

void produce_solution(VERTEX **, int, SEGMENT **, int, int);
VERTEX **setvertex(int);
void setchamber(VERTEX **, int);
SEGMENT **setsegment(int);


#endif