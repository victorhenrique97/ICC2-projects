#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>


//---------------------------------------------------------------------------------------------------//
/*Função com o objetivo de imprimir os indices criados: */
void showindexes(tab_index *data, int indice){

	for(int i=0; i<indice; i++){

		printf("\nIndex information\n");
		printf("\tTablename: %s\n", data[i].index_table);
		printf("\tFieldname: %s\n\n", data[i].index_field);
	}
}
//---------------------------------------------------------------------------------------------------//
/*Função com o objetivo de mostrar estatisticas do SGBD*/
void statistics(stats infos){

	printf("#Tables: %d\n", infos.ntables);
	printf("#Indexes: %d\n", infos.nindexes);
	printf("#Inserts: %d\n", infos.ninserts);
	printf("#Selects: %d\n", infos.nselects);
	printf("#Sorts: %d\n", infos.nsorts);
	printf("#ShowAllTables: %d\n", infos.nshowtab);
	printf("#ShowAllIndexes: %d\n", infos.nshowindex);
	printf("#Records in last select (binary search): %d\n", infos.binlast);
	printf("#Records in last select (sequential search): %d\n", infos.seqlast);
}
//------------------------------------SELECT TABLE----------------------------------------------------//
/*Essa função faz duas buscas, uma no arquivo temporário e outra no arquivo */
void select_table (TAB **t, int tab_count, stats *infos){

	FILE *fp1, *fp2, *fp3;
	char *tablename, *fieldname, *key, *filename1, *filename2, *filename3, *keycontrol;
	int tabposition, desl, sizereg, sizefile, i, j, posicao = -1, fieldsize;
	types fieldtype;

	scanf("%*[ ]"); scanf("%ms", &tablename);
	scanf("%*[ ]"); scanf("%ms", &fieldname);
	scanf("%*[ ]"); scanf("%m[^;\n]", &key);
	keycontrol = key;

	(*infos).binlast=0;
	(*infos).seqlast=0;


	filename1 = calloc(strlen(tablename)+strlen(fieldname)+5, sizeof(char));
	strcpy(filename1, tablename); strcat(filename1, "-"); strcat(filename1, fieldname); strcat(filename1, ".idx");
	filename2 = calloc(strlen(tablename)+4, sizeof(char));
	strcpy(filename2, tablename); strcat(filename2, ".dat");
	filename3 = calloc(strlen(tablename)+4, sizeof(char));
	strcpy(filename3, tablename); strcat(filename3, ".tmp");

	for(i=0; i<tab_count; i++)
		if(!strcmp(tablename, t[i]->t_name)) { //encontrando a posição da tabela e o tamanho do registro
			tabposition = i;
			sizereg = t[i]->reg_size;
		}

	for(i=0; i<t[tabposition]->f_counter; i++) // encontrando a posĩçaõ do campo e o deslocamento
		if(!strcmp(t[tabposition]->f[i]->f_name, fieldname)){
			desl = t[tabposition]->f[i]->f_pos;
			fieldtype = t[tabposition]->f[i]->type;
			fieldsize = t[tabposition]->f[i]->size;
		} 


		if((fieldtype == DOUBLE || fieldtype == FLOAT || fieldtype == INT) && ((key[0] == 39) || (key[0] == 96))){ 
			key+=1;
			strtok(key, "'");   // removendo aspas de inserção caso seja um número uma vez que caracteres já foram armazenados com essas aspas.
		}

	fp1 = fopen(filename1, "r");

	if(fp1 != NULL){
		if(fieldtype == DOUBLE || fieldtype == FLOAT || fieldtype == INT){ // caso o vetor seja desse tipo, faço uma busca binária para números

			double *vec1 = NULL; 
			int *vec2 = NULL;
			double aux1;
			int aux2;
			int i = 0;

			for(i =0; i<t[tabposition]->dat.in_dat; i++){
				fscanf(fp1, "%lf %d", &aux1, &aux2);
				vec1 = (double *)realloc(vec1, sizeof(double)*(i+1));
				vec2 = (int *)realloc(vec2, sizeof(int)*(i+1));
				vec1[i] = aux1;
				vec2[i] = aux2;
			}
			posicao = binary_serach_double(vec1, vec2, 0, i-1, atof(key)); // recebo a posição do registro que contem o valor encocntrado.

			free(vec1);
			free(vec2);
			
			
		}

		else{ // caso o campo a ser procurado seja char, faço uma busca binária com vetores de chars.

			char **vec1 = (char **) calloc(t[tabposition]->dat.in_dat, sizeof(char *)); 
			int *vec2 = (int *) calloc(t[tabposition]->dat.in_dat, sizeof(int));
			int i = 0;

			for(i=0; i<t[tabposition]->dat.in_dat; i++){
				fscanf(fp1, "%m[^,]", &vec1[i]);
				fscanf(fp1, "%*[,]");
				fscanf(fp1, "%d", &vec2[i]);
				fscanf(fp1, "%*[\n]");
			}
			posicao = binary_serach_char(vec1, vec2, 0, i-1, key);
			for(i=0; i<t[tabposition]->dat.in_dat; i++)
				free(vec1[i]);
			free(vec2);
			free(vec1);
		}

		if(posicao != -1){ // caso encontre alguma posição, aloco bytes do tamanho do registro e desloco o ponteiro até a posição e saio imprimindo os elementos
			unsigned char *bytes = (unsigned char *) calloc(sizereg, sizeof(unsigned char));
			unsigned char *aux_bytes;

			fp2 = fopen(filename2, "r");
				fseek(fp2, posicao, SEEK_SET);
				fread(bytes,1, sizereg, fp2);

				aux_bytes = bytes;

				for(i=0; i<t[tabposition]->f_counter; i++){
					if(t[tabposition]->f[i]->type == INT){printf("%d", *((int *)aux_bytes));aux_bytes += 4;}
					if(t[tabposition]->f[i]->type == DOUBLE){printf("%lf", *((double *)aux_bytes)); aux_bytes +=8;}
					if(t[tabposition]->f[i]->type == FLOAT){printf("%f", *((float *)aux_bytes)); aux_bytes +=4;}
					if(t[tabposition]->f[i]->type == CHAR){printf("%s", aux_bytes); aux_bytes +=t[tabposition]->f[i]->size;}
					if(i != t[tabposition]->f_counter -1 )printf(", ");
				}
				printf("\n");
				free(bytes);
			(*infos).binlast++;
			fclose(fp2);
		}
	fclose(fp1);
	}	

	fp3 = fopen(filename3, "r");

	if(fp3 != NULL){ // caso haja algo no temp, carrego esses bytes para a memória e faço uma busca sequencial nele. 
		fseek(fp3, 0, SEEK_END);
		sizefile = ftell(fp3);
		
		rewind(fp3);
		unsigned char *bytes = (unsigned char *) calloc(sizefile, sizeof(unsigned char));
		unsigned char *aux_bytes;
		unsigned char *aux_bytes2;
		fread(bytes, 1, sizefile, fp3);

		aux_bytes = bytes;
		aux_bytes2 = bytes;
		aux_bytes += desl;

		for(i=0; i<t[tabposition]->dat.in_tmp; i++){
			if(fieldtype == INT && atoi(key) == *((int*)aux_bytes)) {
				aux_bytes2 += i*sizereg;
				posicao = i*sizereg;
				for(j=0; j<t[tabposition]->f_counter; j++){
					if(t[tabposition]->f[j]->type == INT){printf("%d", *((int *)aux_bytes2));aux_bytes2 += 4;}
					if(t[tabposition]->f[j]->type == DOUBLE){printf("%lf", *((double *)aux_bytes2)); aux_bytes2 +=8;}
					if(t[tabposition]->f[j]->type == FLOAT){printf("%f", *((float *)aux_bytes2)); aux_bytes2 +=4;}
					if(t[tabposition]->f[j]->type == CHAR){printf("%s", aux_bytes2); aux_bytes2 +=t[tabposition]->f[j]->size;}
					if(j != t[tabposition]->f_counter -1 )printf(", ");
				}
				printf("\n");
				(*infos).seqlast++;
			}
			

			if(fieldtype == FLOAT && atof(key) == *((float*)aux_bytes)) {
				aux_bytes2 += i*sizereg;
				posicao = i*sizereg;
				
				for(j=0; j<t[tabposition]->f_counter; j++){
					if(t[tabposition]->f[j]->type == INT){printf("%d", *((int *)aux_bytes2));aux_bytes2 += 4;}
					if(t[tabposition]->f[j]->type == DOUBLE){printf("%lf", *((double *)aux_bytes2)); aux_bytes2 +=8;}
					if(t[tabposition]->f[j]->type == FLOAT){printf("%f", *((float *)aux_bytes2)); aux_bytes2 +=4;}
					if(t[tabposition]->f[j]->type == CHAR){printf("%s", aux_bytes2); aux_bytes2 +=t[tabposition]->f[j]->size;}
					if(j != t[tabposition]->f_counter -1 )printf(", ");
				}
				printf("\n");
				(*infos).seqlast++;
			}


			if(fieldtype == DOUBLE && atof(key) == *((double*)aux_bytes)) {
				aux_bytes2 += i*sizereg;
				posicao = i*sizereg;
				
				for(j=0; j<t[tabposition]->f_counter; j++){
					if(t[tabposition]->f[j]->type == INT){printf("%d", *((int *)aux_bytes2));aux_bytes2 += 4;}
					if(t[tabposition]->f[j]->type == DOUBLE){printf("%lf", *((double *)aux_bytes2)); aux_bytes2 +=8;}
					if(t[tabposition]->f[j]->type == FLOAT){printf("%f", *((float *)aux_bytes2)); aux_bytes2 +=4;}
					if(t[tabposition]->f[j]->type == CHAR){printf("%s", aux_bytes2); aux_bytes2 +=t[tabposition]->f[j]->size;}
					if(j != t[tabposition]->f_counter -1 )printf(", ");
				}
				printf("\n");
				(*infos).seqlast++;
			}

			if(fieldtype == CHAR && strncmp(key, ((char *)aux_bytes), fieldsize)==0) {
				aux_bytes2 += i*sizereg;
				posicao = i*sizereg;
		
				for(j=0; j<t[tabposition]->f_counter; j++){
					if(t[tabposition]->f[j]->type == INT){printf("%d", *((int *)aux_bytes2));aux_bytes2 += 4;}
					if(t[tabposition]->f[j]->type == DOUBLE){printf("%lf", *((double *)aux_bytes2)); aux_bytes2 +=8;}
					if(t[tabposition]->f[j]->type == FLOAT){printf("%f", *((float *)aux_bytes2)); aux_bytes2 +=4;}
					if(t[tabposition]->f[j]->type == CHAR){printf("%s", aux_bytes2); aux_bytes2 +=t[tabposition]->f[j]->size;}
					if(j != t[tabposition]->f_counter -1 )printf(", ");
				}
				printf("\n");
				(*infos).seqlast++;
			}
		
		aux_bytes +=sizereg;
		aux_bytes2 = bytes;
		}
		fclose(fp3);
		free(bytes);
	}

	if(posicao == -1)printf("null\n"); // caso não encontre nada, imprimo null.

	free(filename1);
	free(filename2);
	free(filename3);
	free(keycontrol);
	free(fieldname);
	free(tablename);

}
//---------------------------------------CREATE INDEX-------------------------------------------------//
void create_index(TAB **t, int t_count, tab_index **data, int *indice){

	char *tablename, *fpname, *fpname2, *fieldname, *fpname3;
	int i, j, n_elem_vec;
	FILE *fp, *fp2, *fp3;
	unsigned char *vector, *aux_vec;
	int desl, numeroelementos, tam_registro;
	types tipo;
	int size_char;


	scanf("%*[ ]");
	scanf("%m[^( ]", &tablename);

	for(i=0; i<t_count; i++)
		if(!strcmp(tablename, t[i]->t_name)){  // encontrando a tabela correspondente
			j = i; 								// encontro o indice da tabela correspondente
			fpname = (char *) calloc((strlen(t[i]->t_name)+ 5), sizeof(char));
			fpname2 = (char *)calloc((strlen(t[i]->t_name)+ 5), sizeof(char));
			strcpy(fpname, t[i]->t_name);
			strcat(fpname, ".tmp");
			strcpy(fpname2, t[i]->t_name);
			strcat(fpname2, ".dat");
		}

		fp = fopen(fpname, "r");

		if(fp != NULL){ // caso haja algo no arquivo temporario, transfiro para o dat. e apago o temp
			fseek(fp, 0, SEEK_END);
			n_elem_vec = ftell(fp);
			rewind(fp);
			
			vector = (unsigned char *)calloc(n_elem_vec, sizeof(unsigned char));
			fread(vector, n_elem_vec, 1, fp);
			fclose(fp);

			fp2 = fopen(fpname2, "a");
			fwrite(vector, 1, n_elem_vec, fp2);
			fclose(fp2);
			remove(fpname);
			free(vector);
		}

		scanf("%*[ (]");
		scanf("%m[^ )]" , &fieldname);
		
		for(i=0; i<t[j]->f_counter; i++){
			if(!strcmp(t[j]->f[i]->f_name, fieldname)){
				numeroelementos = t[j]->num_elem;
				desl = t[j]->f[i]->f_pos;
				tipo = t[j]->f[i]->type;
				tam_registro = t[j]->reg_size;
				size_char = t[j]->f[i]->size;
				
			}
		}

		fp2 = fopen(fpname2, "r"); // abro o arquivo dat e faço uma copia dos dados para memória
		fseek(fp2, 0, SEEK_END);
		n_elem_vec = ftell(fp2);
		rewind(fp2);
		vector = (unsigned char *)calloc(n_elem_vec, sizeof(unsigned char));
		fread(vector, n_elem_vec, 1, fp2);
		fclose(fp2);

		fpname3 = (char *) malloc((strlen(tablename)+strlen(fieldname)+5)*sizeof(char));
		strcpy(fpname3, tablename);
		strcat(fpname3, "-");
		strcat(fpname3, fieldname);
		strcat(fpname3, ".idx");

		
		
		fp3 = fopen(fpname3, "w");

		if(tipo == INT){
			int *vec = (int *) calloc(numeroelementos, sizeof(int));
			int *aux = (int *) calloc(numeroelementos, sizeof(int));

			aux_vec = vector;
			aux_vec += desl;

			for(i=0; i<numeroelementos; i++){
				vec[i] = *((int *)aux_vec);
				aux_vec += tam_registro;
				aux[i] = i*tam_registro;
			}

			bubble_sort_int(vec, aux, numeroelementos);

			for(i=0; i<numeroelementos; i++)
				fprintf(fp3, "%d %d\n", vec[i], aux[i]);

			free(vec);
			free(aux);
			
		}

		if(tipo == FLOAT){
			float *vec = (float *) calloc(numeroelementos, sizeof(float));
			int *aux = (int *) calloc(numeroelementos, sizeof(int));

			aux_vec = vector;
			aux_vec += desl;

			for(i=0; i<numeroelementos; i++){
				vec[i] = *((float *)aux_vec);
				aux_vec += tam_registro;
				aux[i] = i*tam_registro;
			}

			bubble_sort_float(vec, aux, numeroelementos);


			for(i=0; i<numeroelementos; i++)
				fprintf(fp3, "%f %d\n", vec[i], aux[i]);

			free(vec);
			free(aux);

		}

		if(tipo == DOUBLE){
			double *vec = (double *) calloc(numeroelementos, sizeof(double));
			int *aux = (int *) calloc(numeroelementos, sizeof(int));

			aux_vec = vector;
			aux_vec += desl;

			for(i=0; i<numeroelementos; i++){
				vec[i] = *((double *)aux_vec);
				aux_vec += tam_registro;
				aux[i] = i*tam_registro;
			}

			bubble_sort_double(vec, aux, numeroelementos);

			for(i=0; i<numeroelementos; i++)
				fprintf(fp3, "%lf %d\n", vec[i], aux[i]);

			free(vec);
			free(aux);

		}

		if(tipo == CHAR){
			char **vec = (char **) calloc(numeroelementos, sizeof(char *));
			int *aux = (int *) calloc(numeroelementos, sizeof(int));

			for(i=0; i<numeroelementos; i++)
				vec[i] = (char *) calloc(size_char, sizeof(char));

			aux_vec = vector;
			aux_vec += desl;

			for(i=0; i<numeroelementos; i++){
				strcpy(vec[i] ,((char *)aux_vec));
				aux_vec += tam_registro;
				aux[i] = i*tam_registro;
			}

			bubble_sort_char(vec, aux, numeroelementos);

			for(i=0; i<numeroelementos; i++)
				fprintf(fp3, "%s,%d\n", vec[i], aux[i]); // apos ordenar os campos, escrevo no arquivo de indice o valor contido no campo se
														//selecionado, ordenado e o deslocamento do arquvio dat para chegar até esse registo 
			for(i=0; i<numeroelementos; i++)
				free(vec[i]);
			free(vec);
			free(aux);
		}

		t[j]->dat.in_dat += t[j]->dat.in_tmp;
		t[j]->dat.in_tmp = 0;

		fclose(fp3);

		
		int xaux = 0;
		if((*indice)!= 0) // faço uma busca se já há algum indice com o que foi criado, se não houver, guardo o nome do campo e da tabela para utilizar na showallindexes
			for(i=0; i<(*indice); i++)
				if(!strcmp((*data)[i].index_field, fieldname) && !strcmp((*data)[i].index_table, tablename)) xaux = 1;
		
		
		if(!xaux){
			(*data) = (tab_index *)realloc(*data, sizeof(tab_index) * ((*indice)+1));
			(*data)[(*indice)].index_table = tablename;
			(*data)[(*indice)].index_field = fieldname;
			(*indice)++; 
		}

		free(fpname);
		free(fpname2);
		free(fpname3);
		free(vector);
}
//----------------------------------------INSERT--------------------------------------------------//
/*faço inserçoes no arquivo temporario */
void insert(TAB **t, int t_count){

	char *tablename, *aux;
	int i, s_tab, j;
	unsigned char *vector, *vec_aux;
	char *filename;
	FILE *fp;

	scanf("%*[ ]");
	scanf("%ms", &tablename);
	free(tablename);
	scanf("%*[ ]");
	scanf("%m[^( ]", &tablename); // pegando o nome da tabela...

	for(i=0; i<t_count; i++)
		if(!strcmp(tablename, t[i]->t_name)){  // encontrando a tabela correspondente
			s_tab = t[i]->reg_size;
			j = i;
		}

	vector = (unsigned char *) calloc(s_tab, sizeof(unsigned char)); // alocação de bytes
	vec_aux = vector;

	filename = malloc((strlen(tablename)+5)*sizeof(char));
	strcpy(filename, tablename);
	strcat(filename, ".tmp");      //montando o nome do arquivo

	fp = fopen(filename, "a"); 
	if(fp == NULL)

	scanf("%*[ (]");
	scanf("%m[^)]", &aux);
	free(aux);
	scanf("%*[ )]");
	scanf("%m[^(]", &aux);
	free(aux);
	scanf("%*[ (]");

	for(i=0; i<t[j]->f_counter; i++){
		scanf("%m[^,)]" ,&aux); // leio o valor do campo e armazeno ele em um vetor de bytes

		if(t[j]->f[i]->type == INT){
			*((int *) vec_aux) = atoi(aux);
			vec_aux+=4;
		}
		
		else if(t[j]->f[i]->type == FLOAT){
			*((float *)vec_aux) = atof(aux);
			vec_aux+=4;
		}

		if(t[j]->f[i]->type == DOUBLE){
			*((double *) vec_aux) = atof(aux);
			vec_aux+=8;
		}

		if(t[j]->f[i]->type == CHAR){
			memcpy(vec_aux, aux, t[j]->f[i]->size);
			vec_aux += t[j]->f[i]->size;
		}
		scanf("%*[ ,]");
		free(aux);
	}
	fwrite(vector, sizeof(unsigned char), s_tab, fp); // escrevo esse vetor no arquivo temporario
	t[j]->num_elem++;
	t[j]->dat.in_tmp++;
	
	fclose(fp);
	free(filename);
	free(tablename);
	free(vector);

}

//-----------------------------------SHOW ALL TABLES----------------------------------------------//
/*nessa função eu apenas imprimo as tabelas que foram criadas e algumas estatisticas sobre elas */
void show_all_tables(TAB **t, int t_count){

	int i, j;
	char *aux2 = (char *) calloc(10, sizeof(char));
	types aux;

	for(i=0; i<t_count; i++){
		printf("\nTablename: %s\n", t[i]->t_name);
		for(j=0; j<t[i]->f_counter; j++){

				aux = t[i]->f[j]->type;
				if(aux == INT) {strcpy(aux2, "int"); printf("\tField: %s Type: %s Size %d\n", t[i]->f[j]->f_name, aux2,t[i]->f[j]->size);}
				if(aux == FLOAT) {strcpy(aux2, "float"); printf("\tField: %s Type: %s Size %d\n", t[i]->f[j]->f_name, aux2,t[i]->f[j]->size);}
				if(aux == DOUBLE){strcpy(aux2, "double"); printf("\tField: %s Type: %s Size %d\n", t[i]->f[j]->f_name, aux2,t[i]->f[j]->size);}
				if(aux == CHAR && t[i]->f[j]->vec ==0) {strcpy(aux2, "char"); printf("\tField: %s Type: %s Size %d\n", t[i]->f[j]->f_name, aux2,t[i]->f[j]->size);}
				if(aux == CHAR && t[i]->f[j]->vec ==1) {strcpy(aux2, "char"); printf("\tField: %s Type: %s Size %d\n", t[i]->f[j]->f_name, aux2,1);}
				
			}
		printf("\n");
	}
free(aux2);

}
//----------------------------------CREATE TABLE--------------------------------------------------//
/*Nessa função eu crio a tabela*/
TAB **create_table (TAB **t, int *t_count){

	int pos;
	char *aux_name = NULL;
	char *aux_type = NULL;
	int i = 0; // NUMERO DE CAMPOS
	int aux_t; // AUXILIAR DE CAMPOS (TIPO)
	char *size_char = NULL; // VARIAVEL QUE GUARDARA O TAMANHO DO VETOR CASO SEJA CHAR

	pos = *t_count;
	t = (TAB **) realloc(t, sizeof(TAB *)*(pos+1)); 
	t[pos] = (TAB *) calloc(1, sizeof(TAB)); // ALOCAÇÂO DE UMA TABELA

	scanf("%*[ ]");
	scanf("%ms", &t[pos]->t_name);
	scanf("%*[ (]"); // IGNORAR ESPAÇO OU ABERTURA DE PARENTESES


	while(1){

		scanf("%ms", &aux_name); // LENDO O NOME DO CAMPO[0]
		if(aux_name[0] == ')') {free(aux_name);break;}

		t[pos]->f = (FIELD **) realloc(t[pos]->f, sizeof(FIELD *)*(i+1));
		t[pos]->f[i] = (FIELD *) calloc(1, sizeof(FIELD)); // ALOCAÇÂO DE UM CAMPO

		t[pos]->f[i]->f_name = aux_name;

		scanf("%*[ ]"); //IGNORA ESPAÇOS ANTES DO TIPO
		scanf("%m[^ ,)]", &aux_type); // PEGA O TIPO DO CAMPO 
		aux_t = process_type(aux_type);
		
		// obs eu armazeno o char da forma que ele foi digitado, 
		//se tiver aspas guardo tambem com aspas, caso seja apenas um caractere, eu aloco bytes a mais para conter as aspas e o \0

		if(aux_t == -1){
			strtok(aux_type, "["); 
			size_char = strtok(NULL, "]");
			t[pos]->f[i]->size = atoi(size_char);
			t[pos]->f[i]->type = CHAR;
			t[pos]->f[i]->f_pos = t[pos]->reg_size;
			t[pos]->reg_size += atoi(size_char);
			t[pos]->f[i]->vec = 0; 
		}

		else{
			t[pos]->f[i]->vec = 1;
			t[pos]->f[i]->type = aux_t;
			if(t[pos]->f[i]->type == INT) {t[pos]->f[i]->size = sizeof(int); t[pos]->f[i]->f_pos = t[pos]->reg_size; t[pos]->reg_size+= sizeof(int);}
			if(t[pos]->f[i]->type == FLOAT) {t[pos]->f[i]->size = sizeof(float); t[pos]->f[i]->f_pos = t[pos]->reg_size; t[pos]->reg_size+= sizeof(float);}
			if(t[pos]->f[i]->type == DOUBLE) {t[pos]->f[i]->size = sizeof(double); t[pos]->f[i]->f_pos = t[pos]->reg_size; t[pos]->reg_size+= sizeof(double);}
			if(t[pos]->f[i]->type == CHAR) {t[pos]->f[i]->size = 4; t[pos]->f[i]->f_pos = t[pos]->reg_size; t[pos]->reg_size+= 4;}
			
		}
		free(aux_type);
		i++;
		scanf("%*[ ,]"); 
	}

	t[pos]->f_counter = i;
	*t_count +=1;
	

return t;
}
//------------------------------------------------------------------------------------------------//
/*Apenas pego o comando e chamo a função correspondente*/

int main (int argc, char *argv[]){

	char *cmd;
	int op;
	TAB **t = NULL;
	int tab_count = 0;
	stats infos;
	tab_index *data = NULL;
	int indice;
	char *entrada;

	indice = 0;
	infos = empty_stats(); // zero os dados do statistics

	scanf("%ms", &entrada);

	freopen(entrada, "r", stdin);
	free(entrada);

	scanf("%*[ ]"); //ignora os espaços

	while(scanf("%ms", &cmd) != EOF){ // pega a string até o fim do arquivo.
		op = process(cmd); // converto a string para um enum correspondente.
		free(cmd);

		if(op == CREATE){
			scanf("%*[ ]");
			scanf("%ms", &cmd);
			op = process(cmd);
			free(cmd);

			if(op == TABLE){
				t = create_table(t, &tab_count);
				infos.ntables++;
			}
				
			
			else if (op == INDEX){
				create_index(t, tab_count, &data, &indice);
				infos.nindexes++;
			}
		}
		
		else if (op == SHOWALLTABLES){
			show_all_tables(t, tab_count);
			infos.nshowtab++;
		}
		
		
		else if(op == INSERT){
			insert(t, tab_count);
			infos.ninserts++;
		}
			
		else if (op == SELECT){
			select_table(t, tab_count, &infos);
			infos.nselects++;
		}

		else if(op == SORT){
			create_index(t, tab_count, &data, &indice);
			infos.nsorts++;
		}

		else if (op == SHOWALLINDEXES){
			showindexes(data, indice);
			infos.nshowindex++;
		}
		
		else if (op == STATISTICS)
			statistics(infos);

	scanf("%*[^\n]"); // ignore tudo até o fim da linha...
	}

	f_data(data, indice);
	f_table(t, tab_count);

return 0;
}
