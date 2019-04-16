#include <compactar_huffman.h>
#include <descompactar_huffman.h>

/*Função que descobre se a operação será compactação ou descompactação, e chama a função correspondente */

int main(int argc, char *argv[]){

	char *filename, *extension, *fullname;
	
	scanf("%m[^.]", &filename); 
	scanf("%ms", &extension);

	fullname = (char *) calloc(strlen(filename)+strlen(extension)+1, sizeof(char));
	strcat(fullname, filename);
	strcat(fullname, extension);
	
	if(!strcmp(extension, ".txt")) compactar_huff(fullname);
		else if (!strcmp(extension, ".huff")) descompactar_huff(fullname);

	free(filename);
	free(extension);
	free(fullname);
}