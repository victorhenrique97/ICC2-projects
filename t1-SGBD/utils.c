#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
void f_data(tab_index *data, int indice){

	for(int i=0; i<indice; i++){
		free(data[i].index_table);
		free(data[i].index_field);
	}
	free(data);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void f_table(TAB ** t, int counter){

	for(int i =0; i<counter; i++){
		for(int j =0; j<t[i]->f_counter; j++){
		free(t[i]->f[j]->f_name);
		free(t[i]->f[j]);
		}
	free(t[i]->t_name);
	free(t[i]);
	free(t[i]->f);
	}
	free(t);
	
}


//////////////////////////////////////////////////////////////////////////////////////////////////
/*Nessa funçaõ apenas zero os valores do statistics*/
stats empty_stats(){

	stats statstable;

	statstable.ntables = 0;
	statstable.nindexes = 0;
	statstable.ninserts= 0;
	statstable.nselects = 0;
	statstable.nsorts = 0;
	statstable.nshowtab = 0;
	statstable.nshowindex = 0;
	statstable.binlast = 0;
	statstable.seqlast = 0;

return statstable;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

/*Faço uma busca binaria para char */
int binary_serach_char (char **vector, int *vec2, int start, int end, char *key) {
	if (start > end) return -1; // caso base ou condicao de parada da recursao
	int middle = (end + start) / 2;
	if (!strcmp(vector[middle], key)){
		return vec2[middle];
	}
	else if (strcmp(vector[middle], key)<0)
		return binary_serach_char(vector, vec2, middle+1, end, key);
	else return binary_serach_char(vector, vec2, start, middle-1, key);
}
/*Faço uma busca binaria para números */
int binary_serach_double (double *vector, int *vec2, int start, int end, double key) {
	if (start > end) return -1; // caso base ou condicao de parada da recursao
	int middle = (end + start) / 2;
	if (key == vector[middle]){
		return vec2[middle];
	}
	else if (key > vector[middle])
		return binary_serach_double(vector, vec2, middle+1, end, key);
	else return binary_serach_double(vector, vec2, start, middle-1, key);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void bubble_sort_char (char **vec1, int *vec2, int tamanho){

	int aux2, i, j;
	char *aux;

	for(i=tamanho-1; i>0; i--)
		for(j=0; j<i; j++)
			if(strcmp(vec1[j], vec1[j+1])>0){
				aux = vec1[j];
				aux2 = vec2[j];
				vec1[j] = vec1[j+1];
				vec2[j] = vec2[j+1];
				vec1[j+1] = aux;
				vec2[j+1] = aux2;
			}	
}

//---------------------------------------BUBBLE SORT----------------------------------------------//
void bubble_sort_double (double *vec1, int *vec2, int tamanho){

	int aux2, i, j;
	double aux;

	for(i=tamanho-1; i>0; i--)
		for(j=0; j<i; j++)
			if(vec1[j]>vec1[j+1]){
				aux = vec1[j];
				aux2 = vec2[j];
				vec1[j] = vec1[j+1];
				vec2[j] = vec2[j+1];
				vec1[j+1] = aux;
				vec2[j+1] = aux2;
			}	
}

//---------------------------------------BUBBLE SORT----------------------------------------------//
void bubble_sort_int (int *vec1, int *vec2, int tamanho){

	int aux2, i, j;
	int aux;

	for(i=tamanho-1; i>0; i--)
		for(j=0; j<i; j++)
			if(vec1[j]>vec1[j+1]){
				aux = vec1[j];
				aux2 = vec2[j];
				vec1[j] = vec1[j+1];
				vec2[j] = vec2[j+1];
				vec1[j+1] = aux;
				vec2[j+1] = aux2;
			}	
}

//---------------------------------------BUBBLE SORT----------------------------------------------//
void bubble_sort_float (float *vec1, int *vec2, int tamanho){

	int aux2, i, j;
	float aux;

	for(i=tamanho-1; i>0; i--)
		for(j=0; j<i; j++)
			if(vec1[j]>vec1[j+1]){
				aux = vec1[j];
				aux2 = vec2[j];
				vec1[j] = vec1[j+1];
				vec2[j] = vec2[j+1];
				vec1[j+1] = aux;
				vec2[j+1] = aux2;
			}	
}

//-----------------------------------PROCESS TYPE-------------------------------------------------//
/*Converto a string para um enum*/
int process_type(char *string){

	types p_type = -1;

	if(!strcmp(string, "int")) p_type = INT;
	if(!strcmp(string, "float")) p_type = FLOAT;
	if(!strcmp(string, "double")) p_type = DOUBLE;
	if(!strcmp(string, "char")) p_type = CHAR;

return p_type;
}

//------------------------------------PROCESS-----------------------------------------------------//
/*Converto a string para um enum*/
int process(char *cmd){

		func ret;

		if(!strcmp(cmd, "create")) ret = CREATE;
		if(!strcmp(cmd, "insert")) ret = INSERT;
		if(!strcmp(cmd, "select")) ret = SELECT;
		if(!strcmp(cmd, "sort")) ret = SORT;
		if(!strcmp(cmd, "showalltables") || !strcmp(cmd, "showalltables;")) ret = SHOWALLTABLES;
		if(!strcmp(cmd, "statistics"))	ret = STATISTICS;
		if(!strcmp(cmd, "table")) ret = TABLE;
		if(!strcmp(cmd, "index")) ret = INDEX;
		if(!strcmp(cmd, "showallindexes") || !strcmp(cmd, "showallindexes;")) ret = SHOWALLINDEXES; 
		if(!strcmp(cmd, "statistics") || !strcmp(cmd, "statistics;")) ret = STATISTICS; 

return ret;
} 

//------------------------------------------------------------------------------------------------//
void free_table(TAB ** t, int counter){

	for(int i=0; i<counter; i++){
		for(int j=0; j<t[i]->f_counter; j++){
			free(t[i]->f[j]->f_name);
			free(t[i]->f[j]);
		}
		free(t[i]->t_name);
		free(t[i]);
	}
	free(t);

}

//--------------------------------------------------------------------------------------------------//
void free_index(tab_index *data, int indice){

	if(data != NULL){
		for(int i=0; i<indice; i++){
			free(data[i].index_field);
			free(data[i].index_table);
		}
	free(data);
	}
}