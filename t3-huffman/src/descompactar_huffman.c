#include <utils_huffman.h>

////////////////////////////////////////////////////////////////////////////////
/*Função que recebe a mensagem compactada(original) e imprime a descompactada*/
void getting_simbols(No *tree, char *msg_original){

	No *aux = tree;
	char *msg = NULL;
	int msg_len = 0;

	for(int i=0; msg_original[i] != '\0'; i++){	
		if(msg_original[i] == '0') aux = aux->esq;	// percorrendo a arvore...
		else if(msg_original[i] == '1') aux = aux->dir;

		if(aux->esq == NULL && aux->dir == NULL){
			msg = (char *) realloc(msg, (msg_len+1)*sizeof(char)); 
			msg[msg_len] = aux->simbolo[0]; // montando um vetor com a mensagem descompactada...
			msg_len++;
			aux = tree;
		}
	}
	msg = (char *) realloc(msg, (msg_len+1)*sizeof(char));
	msg[msg_len] = '\0';
	
	printf("%s", msg);
	free(msg);
}

////////////////////////////////////////////////////////////////////////////
/*Função que cria a arvore de huffman para a descompactação */
void creating_tree2(No **tree, char simbol, char *code, int level, int len_code){

	if(*tree == NULL) *tree = (No *) calloc(1, sizeof(No)); // alocando um novo nó caso o ponteiro seja nulo
	if(!len_code){
		(*tree)->simbolo = (char *) calloc(2, sizeof(char)); 
		(*tree)->simbolo[0] = simbol;	// nó recebendo o caractere
		(*tree)->simbolo[1] = '\0';
		(*tree)->code = code;

		return;
	}

	else if(code[level] == '0')
		creating_tree2(&(*tree)->esq, simbol, code, ++level, --len_code);
	else creating_tree2(&(*tree)->dir, simbol, code, ++level, --len_code);
}
////////////////////////////////////////////////////////////////////////////
/*Função que le a tabela que auxilia na descompactação e realoca a arvore
de huffman inserindo esse novo "nó".*/
No *creating_nodes2(){

	char simbolo, *code;
	No *tree = NULL;

	scanf("%c", &simbolo);

	while(simbolo != '-'){
		scanf("%*[ -]%ms%*[\n]", &code); // expressão regular que pega o código
	creating_tree2(&tree, simbolo, code, 0, strlen(code));
	scanf("%c", &simbolo);
	}

return tree;
}

/////////////////////////////////////////////////////////////////////////
/*Função que faz a descompactação da mensagem... */
void descompactar_huff(char *filename){

	char *bytes, *binary_code;
	No *tree;
	int msg_start;
	int msg_end;

	freopen(filename, "r", stdin);	// abrindo o arquivo
	tree = creating_nodes2();
	scanf("%*[\n]");
	
	msg_start = ftell(stdin); 
	fseek(stdin, 0, SEEK_END);
	msg_end = ftell(stdin);
	fseek(stdin, msg_start, SEEK_SET);

	binary_code = (char *) calloc(msg_end-msg_start+1, sizeof(char));
	fread(binary_code, msg_end-msg_start, sizeof(char), stdin); // fazendo a leitura do binario
	binary_code[msg_end-msg_start] = '\0';
	
	bytes = byte2vec(binary_code); 	// convertendo bit para bytes
	getting_simbols(tree, bytes); 
	
	free_tree(tree);
	free(binary_code);
	free(bytes);
	fclose(stdin);

}