#ifndef _HASH_H_
#define _HASH_H_

#define M 307187
/*Escolhi um M pouco maior que o número de
entradas para evitar de ter que percorrer a 
hash inteira para encontrar um local vazio*/

typedef struct BUCKETS BUCKETS;

typedef struct HASHTABLE HASHTABLE;

typedef struct LARGESTBUCKETS LARGESTBUCKETS;


struct BUCKETS{
	char **strings;
	int nstrings;
	double code;
};

struct HASHTABLE{
	BUCKETS **buckets;
};

struct LARGESTBUCKETS{
	BUCKETS **buckets;
	int bucketsnumber;
};

HASHTABLE *producehash(char *);

LARGESTBUCKETS *get_buckets(HASHTABLE *);

int compare_strings(const void *, const void *);

int compare_buckets(const void *, const void *);

void destroyhash(HASHTABLE *);


#endif