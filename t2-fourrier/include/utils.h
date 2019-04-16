#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <limits.h>

unsigned char *readbytes(char *, int*);

double complex *fourrier(unsigned char *, int);

double *magnitude(double complex *, int);

int *create_index(int);

void merge_sort(double *, int *, int, int);

void zeroing(double complex *, int *,int, int);

unsigned char *inversa_fourrier(double complex *, int);

int nmag(double *vector, int N);



#endif