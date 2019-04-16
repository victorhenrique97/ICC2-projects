#include <stdlib.h>
#include <stdio.h>
#include <hash.h>

int main(int argc, char *argv[]){


	char *filename = NULL;
	HASHTABLE *hashtable = NULL;
	LARGESTBUCKETS *largestbuckets = NULL;

	scanf("%ms", &filename);

	hashtable = producehash(filename);
	largestbuckets = get_buckets(hashtable);

	for(int i=0; i<largestbuckets->bucketsnumber; i++)
		qsort(largestbuckets->buckets[i]->strings, largestbuckets->buckets[i]->nstrings, sizeof(char *), compare_strings); // ordenando as palavras dos buckets
	qsort(largestbuckets->buckets, largestbuckets->bucketsnumber, sizeof(BUCKETS **), compare_buckets); // ordenando os buckets de acordo com a primeira palavra

	printf("%d\n", largestbuckets->bucketsnumber);
	for(int i=0; i<largestbuckets->bucketsnumber; i++){
		for(int j=0; j<largestbuckets->buckets[i]->nstrings-1; j++)
			printf("%s, ",largestbuckets->buckets[i]->strings[j]);
			printf("%s\n",largestbuckets->buckets[i]->strings[largestbuckets->buckets[i]->nstrings-1]);
	}

	free(largestbuckets->buckets);
	free(largestbuckets);
	destroyhash(hashtable);
	free(filename);

return 0;
}