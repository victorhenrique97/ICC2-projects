#include <utils_huffman.h>
////////////////////////////////////////////////////////////////////////////
/*Função que libera a arvore*/
void free_tree(No *node){

	if(node){
		free_tree(node->esq);
		free_tree(node->dir);
		free(node->simbolo);
		free(node->code);
		free(node);
	}
}
//////////////////////////////////////////////////////////////////////////////
/*Função de ordenação */
void bubble_sort(char *string, int len){

	char aux;

	for(int i=len-1; i>0; i--)
		for(int j=0; j<i; j++)
			if(string[j]>string[j+1]){
				aux = string[j];
				string[j] = string[j+1];
				string[j+1] = aux;
			}
		
}

///////////////////////////////////////////////////////////////////////
/*Função que ordena os nós*/
void bubble_sort_node(No **node, int n){

	No *aux;

	for(int i=n-1; i>0; i--)
		for(int j=0; j<i; j++)
			if((node[j]->freq < node[j+1]->freq) || 
				(node[j]->freq == node[j+1]->freq && (strcmp(node[j]->simbolo, node[j+1]->simbolo)<0))){
				aux = node[j];
				node[j] = node[j+1];
				node[j+1] = aux;
			}
		
}

//////////////////////////////////////////////////////////////////////////////////////
/*Função que faz a conversão de 8 bytes para um byte */
unsigned char *vec2byte(char *string , int uteis){

	int len = strlen(string)/8;
	unsigned char *string_end = (unsigned char *) calloc(len+2,sizeof(unsigned char));

	for(int i=0; i<len; i++)
		for(int j=0; j<8; j++)
			if(string[(i*8)+j] == '1')
				string_end[i] |= (1 << (7-j));

	string_end[len] = uteis;
	string_end[len+1] = '\0';

return string_end;
}

////////////////////////////////////////////////////////////////////////////////////
/*Função que faz a conversão de 1 byte para 8 bytes */
char *byte2vec(char *bytes){

	int msg_len = strlen(bytes);
	int bytes_uteis = bytes[msg_len-1];
	msg_len--;
	int new_len = (msg_len-1)*8+bytes_uteis;
	int k = 0;
	char *vector = (char *) calloc(new_len+1, sizeof(char));

	
	for(int i=0; i<msg_len; i++)
		for(int j=0; j<8 && k<new_len; j++, k++){
			if(bytes[i] & (1 << (7-j))) vector[k] = '1';
			else vector[k] = '0';
		}
		vector[new_len] = '\0';

return vector;	
}

////////////////////////////////////////////////////////////////////////////////////////
/*Função que faz a impressão da tabela que auxilia na descompactação */
void print_code(No *node, char simbol){

	if(node){
		print_code(node->esq, simbol);
		if (node->esq == NULL && node->dir == NULL && node->simbolo[0] == simbol) 
			printf("%s - %s\n", node->simbolo, node->code);
		print_code(node->dir, simbol);
	}

}

