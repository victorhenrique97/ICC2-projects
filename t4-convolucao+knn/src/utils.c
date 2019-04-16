#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <utils.h>
////////////////////////////////////////////////////////////////////////////////
void freeimage(str_images *imagem){

		for(int j=0; j<imagem->nimages; j++){
			free(imagem->imagens[j]->dim);
			free(imagem->imagens[j]);
		}
	
	free(imagem->imagens);
	free(imagem);

}
////////////////////////////////////////////////////////////////////////////////
void freemasks(str_mask *m){

	for(int i=0; i<m->nmask; i++){
		for(int j=0; j<m->maskdim; j++)
			free(m->masks[i].mask[j]);
		free(m->masks[i].mask);
	}

	free(m->masks);
	free(m);
}
////////////////////////////////////////////////////////////////////////////////
void order2(double *class, int *counter, int size){		// ordenação das classes pelo numero de ocorencias

	double aux_class;
	int aux_counter;

	for(int i=size-1; i>0; i--)
		for(int j=0; j<i; j++)
			if(counter[j] < counter[j+1]){
				aux_class = class[j];
				aux_counter = counter[j];
				class[j] = class[j+1];
				counter[j] = counter[j+1];
				class[j+1] = aux_class;
				counter[j+1] = aux_counter;
			}

}
////////////////////////////////////////////////////////////////////////////////
/*Passo as distancias e o numero de vizinhos a serem considerados, e conto o numero
de ocorrencias de classes, ordeno e retorno a que foi mais frquente*/
double resultado(str_KNN *vec_knn, int K){	

	double *vec_class = (double *) calloc(K, sizeof(double));
	int *vec_counter = (int *) calloc(K, sizeof(int));
	int aux_elem = 0, i = 0, j = 0;
	double result;

	for(i=0; i<K; i++){
		for(j=0; j<aux_elem; j++){
			if(vec_class[j] == vec_knn[i].classe){
				vec_counter[j]++; 
				break;
			}
		}
		if(j == aux_elem){
			vec_class[j] = vec_knn[i].classe;
			vec_counter[j] = 1;
			aux_elem++;
		}
	}
	order2(vec_class, vec_counter, aux_elem);

	result = vec_class[0];
	free(vec_class);
	free(vec_counter);

return result;
}
/////////////////////////////////////////////////////////////////////////////////
void order(str_KNN *vec_knn, int size){				//ordeno em ordem crescente de distancias

	str_KNN aux;

	for(int i=size-1; i>0; i--)
		for(int j=0; j<i; j++)
			if(vec_knn[j].dist > vec_knn[j+1].dist){
				aux = vec_knn[j];
				vec_knn[j] = vec_knn[j+1];
				vec_knn[j+1] = aux;
			}
}
/////////////////////////////////////////////////////////////////////////////////
double distancia(double *vectorA, double *vectorB, int dim){

	double dist = 0;
	for(int i=0; i<dim; i++)
	dist += pow((vectorA[i] - vectorB[i]), 2);
	dist = sqrt(dist);

return dist;
}

/////////////////////////////////////////////////////////////////////////////////
double *KNN(str_images *training, str_images *testing, int K){

	str_KNN *vec_knn = (str_KNN *) calloc(training->nimages, sizeof(str_KNN));
	double *vector = (double *) calloc(testing->nimages, sizeof(double));

	for(int i=0; i<testing->nimages; i++){	// percorro o conjunto de teste
		for(int j=0; j<training->nimages; j++){	// percorro o conjunto de treinamento
			vec_knn[j].dist = distancia(training->imagens[j]->dim, // calculo as distancias 
			testing->imagens[i]->dim, training->imagens[i]->ndim); //de um ponto de teste a todos do treinamento
			vec_knn[j].classe = training->imagens[j]->classe;	  
		}
		order(vec_knn, training->nimages);	// ordeno distancias menor para maior
		vector[i] = resultado(vec_knn, K);	// ordeno por frequencias Maior para menor
	}

	free(vec_knn);

return vector;		// retorno um vetor resposta
}
/////////////////////////////////////////////////////////////////////////////////
double **callocmatrix(int row, int col){

	double **matrix = (double **)calloc(row, sizeof(double *));

	for(int i=0; i<row; i++)
		matrix[i] = (double *) calloc(col, sizeof(double));

return matrix;
}
//////////////////////////////////////////////////////////////////////////////////
double **creatematrix(unsigned char *vector, int nrow, int ncol){

	double **matrix = callocmatrix(nrow, ncol);
	int k=0;

	for(int i=0; i<nrow; i++)
		for(int j=0; j<ncol; j++)
			matrix[i][j] = vector[k++];

return matrix;
}
//////////////////////////////////////////////////////////////////////////////////
void freematrix(double **matrix, int nrow){

	for(int i=0; i<nrow; i++)
		free(matrix[i]);
	free(matrix);
}
//////////////////////////////////////////////////////////////////////////////////
double funcentropia(double *vector, int nelem){

	double entropia = 0;

	for(int i=0; i<nelem; i++)
		entropia += (vector[i] * (log(fabs(vector[i])+1)/log(2)));
	entropia *= -1;

return entropia;
}
/////////////////////////////////////////////////////////////////////////////////
double funcvariancia(double *vector, int nelem, double media){

	double variancia = 0;
	for(int i=0; i<nelem; i++)
		variancia += pow((vector[i]-media), 2);
	variancia /= nelem;

return variancia;
}
/////////////////////////////////////////////////////////////////////////////////
double funcmedia(double *vector, int nelem){

	double media = 0;

	for(int i=0; i<nelem; i++)
		media += vector[i];
	media /= nelem;

return media;
}
//////////////////////////////////////////////////////////////////////////////////
/*Função responsavel por obter as caracteristicas de uma matrix*/
double *caracteristicas(double **matrix, int nrow, int ncol){

	double positive = 0, negative = 0, nulo = 0, media = 0; 
	int k = 0;

	double *result = (double *) calloc(nrow*6, sizeof(double));

	for(int i=0; i<nrow; i++){
		for(int j=0; j<ncol; j++){
			if(matrix[i][j] == 0) nulo++;
			else if(matrix[i][j] > 0) positive++;
			else  negative++;
		}
		result[k++] = positive;
		result[k++] = nulo;
		result[k++] = negative;
		media  = funcmedia(matrix[i], ncol);
		result[k++] = media;
		result[k++] = funcvariancia(matrix[i], ncol, media);
		result[k++] = funcentropia(matrix[i], ncol);
		positive = negative = nulo = media = 0;
	}

return result;
}
/////////////////////////////////////////////////////////////////////////////////
double convolution2(double **matrix, int row1, int col1, int i, int j, double **mask, int dim){

	int aux = dim/2;
	double sum = 0;
	i-= aux;
	j -= aux;

	for(int k=0; k<dim; k++)
		for(int l=0; l<dim; l++)
			if(i+k >= 0 && i+k <row1 && j+l >=0 && j+l <col1)
				sum += (matrix[i+k][j+l] * mask[k][l]);

return sum;

}
/////////////////////////////////////////////////////////////////////////////////
double **convolution(double **matrix, int row1, int col1, double **mask, int dim){

	double **matrix_result = callocmatrix(row1, col1);

	for(int i=0; i<row1; i++)
		for(int j=0; j<col1; j++)
			matrix_result[i][j] = convolution2(matrix, row1, col1, i, j, mask, dim);

return matrix_result;
}

//////////////////////////////////////////////////////////////////////////////////
vec_image *training_image(hfile *headfile, str_mask *m, double class, int flag){

	double **matrix_result = NULL;
	double *vector_result = NULL;
	double *vector_aux = NULL;
	int k =0;
	vec_image *imagem = (vec_image *) calloc(1, sizeof(vec_image));


	for(int i=0; i<m->nmask; i++){
		matrix_result = convolution(headfile->matrix, headfile->nrow, headfile->ncol, m->masks[i].mask, m->maskdim); // aplico a convolução
		vector_aux = caracteristicas(matrix_result, headfile->nrow, headfile->ncol);									// pego as caracteristcas
		freematrix(matrix_result, headfile->nrow);
		vector_result = (double *) realloc(vector_result, (headfile->nrow)*6*(i+1)*sizeof(double));
		for(int j=0; j<(headfile->nrow)*6; j++)
			vector_result[k++] = vector_aux[j];
		free(vector_aux);
	}

	imagem->dim = vector_result;
	imagem->ndim = k;
	if(flag) imagem->classe = class;

return imagem;
}

//////////////////////////////////////////////////////////////////////////////////
vec_image *open_image(char *image_name, str_mask *m, double class, int flag){

	FILE *fp_image;
	int start, end, len;
	hfile *headfile;
	unsigned char *vector;
	vec_image *imagem = NULL;

	headfile = (hfile *) malloc(1*sizeof(hfile));
	fp_image = fopen(image_name, "r");

	fscanf(fp_image,"%ms%*[ \n]", &(headfile->magic_number));
	fscanf(fp_image,"%d%*[ \n]%d%*[ \n]", &(headfile->ncol), &(headfile->nrow));
	fscanf(fp_image,"%d%*[ \n]", &(headfile->max_value));

	start = ftell(fp_image);
	fseek(fp_image, 0, SEEK_END);
	end = ftell(fp_image);
	fseek(fp_image, start, SEEK_SET);
	len = (end-start);

	vector = (unsigned char*)malloc(len*sizeof(unsigned char));
	fread(vector, sizeof(char), len, fp_image);
	fclose(fp_image);

	headfile->matrix = creatematrix(vector, headfile->nrow, headfile->ncol);
	free(vector);
	imagem = training_image(headfile, m, class, flag);

	freematrix(headfile->matrix, headfile->nrow);
	free(headfile->magic_number);
	free(headfile);

return imagem;
}
/////////////////////////////////////////////////////////////////////////////////
str_images *getting_images(str_mask *mascaras, char *filename, char *fileclassname){

	FILE *fp_training = NULL;
	FILE *fp_trainingclass = NULL;
	str_images * images = (str_images *)calloc(1, sizeof(str_images));
	char *image_name = NULL;
	double class = 0;
	int counter = 0;
	int flag = 0;

	if(filename) fp_training = fopen(filename, "r");
	if(fileclassname) fp_trainingclass = fopen(fileclassname, "r");

	while(fscanf(fp_training, "%ms%*[ \n]", &image_name) != EOF){
		if(fp_trainingclass){
			fscanf(fp_trainingclass, "%lf%*[ \n]", &class);
			flag = 1;
		}
		images->imagens = (vec_image **) realloc(images->imagens, (counter+1)*sizeof(vec_image *));
		images->imagens[counter++] = open_image(image_name, mascaras, class, flag);
		free(image_name);
	}

	images->nimages = counter;
	fclose(fp_training);
	if(fp_trainingclass) fclose(fp_trainingclass);
		
return images;
}
/////////////////////////////////////////////////////////////////////////////////
matrix_mask *create_masks(double *vector, int nmask, int maskdim){

	matrix_mask *masks = (matrix_mask *) malloc(nmask*sizeof(matrix_mask));
	int pos = 0;

	for(int i=0; i<nmask; i++){
		masks[i].mask = (double **)malloc(maskdim*sizeof(double *));
		for(int j=0; j<maskdim; j++)
			masks[i].mask[j] = (double *)malloc(maskdim*sizeof(double));
	}

	for(int i=0; i<nmask; i++)
		for(int j=0; j<maskdim; j++)
			for(int k=0; k<maskdim; k++)
				masks[i].mask[j][k] = vector[pos++];

return masks;
}
/////////////////////////////////////////////////////////////////////////////
/*Função para a leitura da entrada, que chama internamente outra para transformar o vetor em matrix*/
matrix_mask *read_masks(int nmask, int maskdim){

	int lines = maskdim*maskdim*nmask;
	matrix_mask *masks = NULL;
	double *vector = (double *) malloc(lines*sizeof(double));

	for(int i=0; i<lines; i++)
		scanf("%lf%*[ \n]", &vector[i]);
	masks = create_masks(vector, nmask, maskdim);

	free(vector);

return masks;
}