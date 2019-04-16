#include <utils.h>

int main(int argc, char *argv[]){

	char *filename;
	int C, size, *vector_index, n_magnitudes_relevantes;
	unsigned char *bytes, *improved_bytes;						
	double complex *vector_fourrier;
	double *vector_magnitude;

	scanf("%ms %d", &filename, &C);
	bytes = readbytes(filename, &size);
	
	vector_fourrier = fourrier(bytes, size);
	vector_magnitude = magnitude(vector_fourrier, size/2+1);
	vector_index = create_index(size/2+1);
	n_magnitudes_relevantes = nmag(vector_magnitude, size/2+1);
	merge_sort(vector_magnitude, vector_index, 0, size/2);
	zeroing(vector_fourrier, vector_index, size/2+1, C);
	improved_bytes = inversa_fourrier(vector_fourrier, size);

	printf("%d\n%d\n", size, n_magnitudes_relevantes);
	for(int i =0; i<C; i++)
		printf("%d ", (int)vector_magnitude[i]);
	printf("\n");
	for(int i=0; i<size; i++)
		printf("%d\n", (int)improved_bytes[i]);

	free(filename);
	free(bytes);
	free(vector_magnitude);
	free(vector_index);
	free(vector_fourrier);
	free(improved_bytes);

return 0;
}
