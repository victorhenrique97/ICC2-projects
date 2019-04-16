#ifndef _UTILS_H_
#define _UTILS_H_

#define TRUE '1'
#define FALSE '0'

#include <maze_runner.h>

void vector_inversor(int *, int);
float euclidean_distance(float, float, float, float);
void pop(STACK *);
void push(STACK **, int, int);
SEGMENT **allocsegment(int);
VERTEX **allocvertex(int);
void free_solution(SOLUTION **, int);
void free_vertex(VERTEX **, int);
void free_segment(SEGMENT **, int);

#endif