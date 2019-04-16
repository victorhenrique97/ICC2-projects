#include <compactar_huffman.h>

//////////////////////////////////////////////////////////////////////////////////////////
/*Função que recebe um nó da arvore e um simbolo, se o simbolo contido na arvore
for igual ao que foi passado, copio esse simbolo para um vetor.*/

void returnCode(No *node, char simbol, char **ret_code){

	if(node){
		returnCode(node->esq, simbol, ret_code);
		if (node->esq == NULL && node->dir == NULL && node->simbolo[0] == simbol){ 
			*ret_code = (char *) calloc(strlen(node->code)+1, sizeof(char));
			strcpy(*ret_code, node->code);
		}
		returnCode(node->dir, simbol, ret_code);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
/*Função que recebe a arvore de Huffman, os simbolos e compacta a mensagem*/
void getting_numbers(No *tree, char *msg_simbols, char *msg, int len_msg_simbols, int len_msg){


	int bytes_size = 0, bytes_uteis, bytes_to_complete = 0, msg_compacted_len;
	char *bytes = NULL, *ret_code = NULL;
	unsigned char *msg_compacted;
	
	returnCode(tree, msg[0], &ret_code); // pegando o código da letra
	bytes_size += strlen(ret_code);
	bytes = (char *) calloc(bytes_size+1,sizeof(char));
	memcpy(bytes,ret_code,bytes_size);
	free(ret_code);

	for(int i=1; i<len_msg; i++){
		returnCode(tree, msg[i], &ret_code);
		bytes_size += strlen(ret_code);
		bytes = (char *) realloc(bytes, sizeof(char)*(bytes_size+1));	 
		strcat(bytes, ret_code); // concateno os bytes retornados com uma string que conterá todos os bytes da mensagem									 
		free(ret_code);	
	}

	bytes_uteis = bytes_size%8;

	if(bytes_uteis){	// caso seja necessário, preencho o que faltar com zeros.
		bytes_to_complete = 8-bytes_uteis;
		bytes = (char *) realloc(bytes,(bytes_size+bytes_to_complete+1)*sizeof(char));
		for(int i=bytes_size; i<bytes_size+bytes_to_complete; i++)
			bytes[i] = '0';
	}
	bytes[bytes_size+bytes_to_complete] = '\0';

	for(int i=0; i<len_msg_simbols; i++)		// fazendo a impressão da tabela que auxilia na descompactação
	print_code(tree, msg_simbols[i]);
	printf("-\n");
	msg_compacted = vec2byte(bytes, bytes_uteis); // convertendo de bytes para bits
	
	msg_compacted_len = strlen((char *)msg_compacted);	// imprimindo os valores dos caracteres ASCII
	for(int i=0; i<msg_compacted_len; i++)
	printf("%d\n", msg_compacted[i]);
	
	free(msg_compacted);
	free(bytes);

}
//////////////////////////////////////////////////////////////////////////////////////////
/*Função que seta os valores (zeros e uns) no nó correspondente */
void setting_values(No *node, char *vec_bool, int level){

	if(node->esq == NULL && node->dir == NULL){
		node->code = (char *) calloc(level+1, sizeof(char));
		for(int i=0; i<level; i++) 
			node->code[i] = vec_bool[i];
		node->code[level] = '\0';
	}

	else {
		if(node->esq != NULL){
			vec_bool[level] = '0';
			setting_values(node->esq, vec_bool, level+1);
		}

		if(node->dir != NULL){
			vec_bool[level] = '1';
			setting_values(node->dir, vec_bool, level+1);
		}
	}	
} 
////////////////////////////////////////////////////////////////////////////////////
/*Função que recebe um ponteiro de nós e monta a arvore de huffman */
No* creating_tree1(No **node, int n_nodes){

	No *tree = NULL, *aux = NULL;
	int simbol_size;
	char *vec_aux;

	while(n_nodes>1){
		
		bubble_sort_node(node, n_nodes); // ordenando o vetor de nós...

		aux = (No *) calloc(1, sizeof(No));
		simbol_size = strlen(node[n_nodes-1]->simbolo) + strlen(node[n_nodes-2]->simbolo) + 1; 
		aux->simbolo = (char *) calloc(simbol_size, sizeof(char)); // alocando memória para o novo simbolo

		strcat(aux->simbolo, node[n_nodes-1]->simbolo);	// concatenando os simbolos de menor ocorrencia...
		strcat(aux->simbolo, node[n_nodes-2]->simbolo);
		aux->freq = node[n_nodes-1]->freq + node[n_nodes-2]->freq;
		aux->esq = node[n_nodes-1]; 
		aux->dir = node[n_nodes-2];
		node[n_nodes-2] = aux; // colocando o novo nó criado no vetor
		n_nodes--; 
		
		tree = node[n_nodes-1];	// setando um ponteiro para o nó raiz
	}
	
	vec_aux = (char *) calloc(9, sizeof(char));
	setting_values(tree, vec_aux, 0);
	free(vec_aux);

return tree;
}

///////////////////////////////////////////////////////////////////////////////////////////
/*Função que recebe uma mensagem, e retorna um vetor com os simbolos que continham na mensagem e 
uma arvore de huffman criada */
No* creating_nodes1(char *msg, char **msg_simbols){

	No **node = NULL;
	No *tree;
	int msg_len = strlen(msg);
	int n_nodes = 0, j;

	for(int i=0; i<msg_len; i++){ // caso já haja um simbolo correspondente no vetor...
		for(j=0; j<n_nodes; j++){
			if(node[j]->simbolo[0] == msg[i] && node[j]->freq != 0){
				node[j]->freq++;
				break;
			}
		}

		if(j == n_nodes){	// caso não haja um simbolo correspondente no vetor...
				node = (No **) realloc(node, sizeof(No *)*(n_nodes+1));
				node[n_nodes] = (No *) calloc(1, sizeof(No));
				node[n_nodes]->simbolo = calloc(2, sizeof(char));
				node[n_nodes]->simbolo[0] = msg[i]; 
				node[n_nodes]->simbolo[1] = '\0';
				node[n_nodes]->freq++;
				*msg_simbols = (char *) realloc(*msg_simbols, (n_nodes+1)*sizeof(char));
				(*msg_simbols)[n_nodes] = msg[i];
				n_nodes++;
			}
	}

	*msg_simbols = (char *) realloc(*msg_simbols, (n_nodes+1)*sizeof(char));
	(*msg_simbols)[n_nodes] = '\0';
	tree = creating_tree1(node, n_nodes);
	free(node);

return tree;
}

//////////////////////////////////////////////////////////////////////
/*Função que faz a compactação*/
void compactar_huff(char *filename){
	
	char *msg, *msg_simbols = NULL;
	int size_of_file;
	FILE *fp;
	No *tree;

	fp = fopen(filename, "r"); 		// abertura do arquivo
	fseek(fp, 0, SEEK_END);
	size_of_file = ftell(fp);
	rewind(fp);
	msg = (char *) calloc(size_of_file+1, sizeof(char));
	fread(msg, sizeof(char), size_of_file, fp);	// leitura dos bytes
	msg[size_of_file] = '\0';
	fclose(fp);

	tree = creating_nodes1(msg, &msg_simbols);
	bubble_sort(msg_simbols, strlen(msg_simbols));
	getting_numbers(tree, msg_simbols, msg, strlen(msg_simbols), size_of_file);

	free_tree(tree);
	free(msg_simbols);
	free(msg);
}
