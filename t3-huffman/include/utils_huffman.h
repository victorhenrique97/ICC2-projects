#ifndef _UTILS_HUFFMAN_H_
#define _UTILS_HUFFMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct No No;

	struct No{
	char *simbolo;
	char *code;
	int freq;
	No *esq;
	No *dir;
};

void bubble_sort(char *, int);
void bubble_sort_node(No **, int);
unsigned char *vec2byte(char *, int);
void print_code(No *, char);
char *byte2vec(char *);
unsigned char *vec2byte(char *, int);
void free_tree(No *tree);


#endif