#include <stdlib.h>
#include <stdio.h>
#include <utils.h>

int main(int argc, char *argv[]){

	str_mask *m = (str_mask *) calloc(1, sizeof(str_mask));
	str_images *img_trained = NULL;
	str_images *img_testing = NULL;
	char *training_file, *trainingclass_file, *testing_file;
	double *result = NULL;
	int K;

	scanf("%ms", &training_file);
	scanf("%ms%*[ \n]", &trainingclass_file);
	scanf("%ms%*[ \n]", &testing_file);
	scanf("%d%*[ \n]", &(m->nmask));
	scanf("%d%*[ \n]", &(m->maskdim));
	m->masks = read_masks(m->nmask, m->maskdim);							// armazenando as mascaras...
	img_trained = getting_images(m, training_file, trainingclass_file);		//armazenando as imagens de treinamento
	img_testing = getting_images(m, testing_file, NULL);					// armazenando as imagens de teste
	scanf("%d%*[ \n]", &K);
	result = KNN(img_trained, img_testing, K);								// aplicando o KNN

	for(int i=0; i<img_testing->nimages; i++)
		printf("%.3f\n", result[i]);

	free(training_file);
	free(trainingclass_file);
	free(testing_file);
	freemasks(m);
	freeimage(img_trained);
	freeimage(img_testing);
	free(result);

return 0;
}