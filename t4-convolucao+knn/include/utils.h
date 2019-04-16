#ifndef _UTILS_H_
#define _UTILS_H_


typedef struct{			// struct que armazena a imagem
	int nrow, ncol;
	int max_value;
	unsigned char *magic_number;
	double **matrix;
}hfile;

typedef struct{			// struct para guardar uma mascara
	double **mask;
}matrix_mask;

typedef struct{			// struct para guardar as mascaras
	int nmask;
	int maskdim;
	matrix_mask *masks;
}str_mask;

typedef struct{			// struct para guardar um vetor de caracteristicas
	double classe;
	double *dim;
	int ndim;
}vec_image;

typedef struct{			// struct para guardar os vetores de caracteristicas
	vec_image **imagens;
	int nimages;
	int flag;
}str_images;

typedef struct{			// struct para guardar as distancias de um ponto ao conjunto de treinamento
	double dist;
	double classe;
}str_KNN;


matrix_mask *read_masks(int, int);
str_images *getting_images(str_mask *, char *, char *);
double *KNN(str_images *, str_images *, int);
void freemasks(str_mask *);
void freeimage(str_images *);

#endif