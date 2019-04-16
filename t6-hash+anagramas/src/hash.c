#include <stdlib.h>
#include <stdio.h>
#include <hash.h>
#include <string.h>
#include <limits.h>
///////////////////////////////////////////////////////////////////////
void destroyhash(HASHTABLE *hashtable){ // desaloca a hash

	for(int i=0; i<M; i++){
		for(int j=0; j<hashtable->buckets[i]->nstrings; j++)
			free(hashtable->buckets[i]->strings[j]);
		free(hashtable->buckets[i]->strings);
		free(hashtable->buckets[i]);
	}
	free(hashtable->buckets);
	free(hashtable);
}
///////////////////////////////////////////////////////////////////////
int compare_strings(const void *A, const void *B){ // compara strings

	return strcmp(*(char * const*)A, *(char * const*)B);
}
///////////////////////////////////////////////////////////////////////
int compare_buckets(const void *A, const void *B){ // compara buckets de acordo com a primeira string

	return strcmp((*(BUCKETS **)A)->strings[0], (*(BUCKETS **)B)->strings[0]);
}
/////////////////////////////////////////////////////////////////////////
LARGESTBUCKETS *get_buckets(HASHTABLE *hashtable){ // obtenho os buckets

	int bigger = INT_MIN;
	int counter = 0;
	LARGESTBUCKETS *largestbuckets = (LARGESTBUCKETS *) calloc(1, sizeof(LARGESTBUCKETS));

	for(int i=0; i<M; i++){
		if(hashtable->buckets[i]->nstrings > bigger){ // caso seja maior, zero e começo a armazenar as palavras denovo
			bigger = hashtable->buckets[i]->nstrings;
			counter = 0;
		}
		if(hashtable->buckets[i]->nstrings == bigger){ // se for igual adciono um bucket novo
			largestbuckets->buckets = (BUCKETS **) realloc(largestbuckets->buckets, sizeof(BUCKETS *)*(counter+1));
			largestbuckets->buckets[counter++] = hashtable->buckets[i];
		}
	}
	largestbuckets->bucketsnumber = counter;

return largestbuckets;
}
/////////////////////////////////////////////////////////////////////////
double getfullhashcode(char *string){ // pego o código completo da palvra ignorando o '-'

	int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};
	double hash = 1;
	int len = strlen(string);

	for(int i=0; i<len; i++)
		if(string[i] != '-')
			hash *= primes[string[i]%('z'-'a'+1)];
		
return hash;
}
/////////////////////////////////////////////////////////////////////////
int gethashcode(char *string){ // descubro a possivel posição da hash

	int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};
	int hash = 1;
	int len = strlen(string);
	
	for(int i=0; i<len; i++)
		hash = (hash * primes[string[i]%('z'-'a'+1)])%M;

return hash;
}
/////////////////////////////////////////////////////////////////////////
void hashing(HASHTABLE *hashtable, char *string){

	int pos = gethashcode(string);
	double code = getfullhashcode(string);

	while(hashtable->buckets[pos]->code != 0 && hashtable->buckets[pos]->code != code)
		pos = pos+1; // caso já haja palavras no bucket e sejem diferentes, procuro a próxima posição livre
	hashtable->buckets[pos]->code = code;
	hashtable->buckets[pos]->strings = (char **)realloc(hashtable->buckets[pos]->strings, sizeof(char *)*(hashtable->buckets[pos]->nstrings+1));
	hashtable->buckets[pos]->strings[hashtable->buckets[pos]->nstrings++] = string; // coloco a string no bucket
}
/////////////////////////////////////////////////////////////////////////
HASHTABLE *callochash(int nelem){ // alloco a hash

	HASHTABLE *hashtable = (HASHTABLE *) malloc(sizeof(HASHTABLE));
	hashtable->buckets = (BUCKETS **) malloc(nelem*sizeof(BUCKETS *));

	for(int i=0; i<nelem; i++)
		hashtable->buckets[i] = (BUCKETS *) calloc(1, sizeof(BUCKETS));

return hashtable;
}
/////////////////////////////////////////////////////////////////////////
HASHTABLE *producehash(char *filename){ // abro o arquivo e chamo a função que gera a hash

	char *stringaux;
	HASHTABLE *hashtable = callochash(M);
	FILE *fp = fopen(filename, "r");

	while(fscanf(fp, "%ms", &stringaux) != EOF){
		hashing(hashtable, stringaux);
	}
	fclose(fp);

return hashtable;
}