#include <utils.h>

//-------------------------------------READBYTES--------------------------------//
/*Função que le os bytes do arquivo e armazena em um vetor de unsigned char */
unsigned char *readbytes(char *filename, int *file_length){


	FILE *fp;
	unsigned char *bytes;


	fp = fopen(filename, "r");
	fseek(fp, 0, SEEK_END);
	(*file_length) = ftell(fp);
	rewind(fp);
	bytes = (unsigned char *) malloc((*file_length) * sizeof(unsigned char));
	fread(bytes, sizeof(unsigned char), (*file_length), fp);
	fclose(fp);

return bytes;
}

//----------------------------------FOURRIER-----------------------------------//
/*Função que aplica a transformada discreta de fourrier*/
double complex *fourrier(unsigned char *bytes, int N){


	double complex *vector = (double complex *) calloc(N/2+1, sizeof(double complex)); // aloco N/2+1 pois o somatório varia de 0 até N/2
	
	for(int k = 0; k<=N/2; k++){ 
		for(int t = 0; t<N; t++)
			vector[k] += bytes[t] * cexp(-2*M_PI*I*k*t/N);
		if(k==0) vector[k] *= 1.0/N; 						//caso k seja zero multiplico por 1.0  
		else vector[k] *= 2.0/N; 							// caso contrário multiplico por 2.0
	}

return vector;
}

//---------------------------------MAGNITUDE-----------------------------------//
/*Função que calcula a magnitude (módulo de um número complexo) */
double *magnitude(double complex *coef, int N){

	double *vector = (double *) malloc(N* sizeof (double));

	for(int k=0; k<N; k++)
		vector[k] = sqrt(pow(creal(coef[k]),2) + pow(cimag(coef[k]),2)); // distância euclidiana entre a parte real e imaginaria,
																		 // tomando como segundo ponto a origem.
return vector;
}

//-----------------------------CREATE INDEX (T1 feelings)--------------------//
/*Função que cria um índice*/
int *create_index(int N){

	int * vector = (int *) malloc(N*sizeof(int));

	for(int i=0; i<N; i++)
		vector[i] = i;

return vector;
}

//--------------------------------MERGE--------------------------------------//
/* Segunda parte da função que faz a ordenação */
void merge(double *vec1, int *vec2, int start, int middle, int end) {

	double *left1, *right1;
	int *left2, *right2;
	int nleft, nright, i, j, k;

	nleft = middle - start + 2;
	nright = end - middle + 1;
	
	left1 = (double *) malloc(sizeof(double) * nleft);			// aloco uma posição a mais para colocar o setinela
	right1 = (double *) malloc(sizeof(double) * nright);		// idem
	left2 = (int *) malloc(sizeof(int) * (nleft-1));
	right2 = (int *) malloc(sizeof(int) * (nright-1));


	for (k=0, i = start; i <= middle; i++, k++){
		left1[k] = vec1[i];
		left2[k] = vec2[i];
	}	
	left1[k] = INT_MIN;	//ultima posição recebe o sentinela
				

	for (k=0, j = middle+1; j <= end; j++, k++) {
	right1[k] = vec1[j];
	right2[k] = vec2[j];
	}
	right1[k] = INT_MIN; //ultima posição recebe o sentinela

	i = j = 0;

	for (k = start; k <= end; k++) {				
		if (left1[i] > right1[j]) {
			vec1[k] = left1[i];
			vec2[k] = left2[i++];

		} else {
			vec1[k] = right1[j];
			vec2[k] = right2[j++];
		}
	}

	free(left1);
	free(left2);
	free(right1);
	free(right2);
}
//-------------------------------MERGE SORT----------------------------------//
/*Primeira parte da função que faz a ordenação*/
void merge_sort(double *vec1, int *vec2, int start, int end){

		int middle;

	if (start < end) {
		middle = (start + end) / 2;
		merge_sort(vec1, vec2, start, middle); //chamo passando a primeira metade do vetor
		merge_sort(vec1, vec2, middle+1, end); //chamo passando a segunda metade do vetor
		merge(vec1, vec2, start, middle, end); //chamo a segunda parte da função responsável por ordenar os elementos
		
	}
}

//--------------------------------ZEROING---------------------------------------//
/*Funão que zera os coeficientes cujas posições forem maiores do que C*/
void zeroing(double complex *vec1, int *vec2, int N, int C){

	for(int i = C; i<N; i++)
		vec1[vec2[i]] = 0;
}
//----------------------------INVERSA FOURRIER----------------------------------//
/*Função que aplica a transformada discreta inversa de fourrier */
unsigned char *inversa_fourrier(double complex *vector, int N){

	double complex *aux = (double complex *) calloc(N, sizeof(double complex));
	unsigned char *bytes = (unsigned char *) calloc (N, sizeof(unsigned char));

	for(int t = 0; t<N; t++){
		for(int k = 0; k<=N/2; k++)
			aux[t] += vector[k] * cexp(I*k*2*M_PI*t/N);
		bytes[t] = ((unsigned char)(round(aux[t]))); //arredondo o número e uso um cast para unsigned char
	}

	free(aux);
return bytes;
}

//-----------------------------NMAG---------------------------------------------//
/*Função que conta ocorrencias de números maiores do que 0.1*/
int nmag(double *vector, int N){

	int num = 0;
	for(int i=0; i<N; i++)
		if(vector[i]>0.1) num++;

return num;
}