#ifndef _UTILS_H_
#define _UTILS_H_


///////////////////////
typedef enum{
	INT,
	DOUBLE,
	FLOAT,
	CHAR,
}types;

typedef enum{
	CREATE,
	INSERT,
	SELECT,
	SORT,
	SHOWALLTABLES,
	STATISTICS,
	TABLE,
	INDEX,
	SHOWALLINDEXES,
}func;
//////////////////////

typedef struct{
	char *f_name;
	types type;
	int size;
	int f_pos;
	int vec; 
}FIELD;

typedef struct{
	int in_tmp;
	int in_dat;
}data;

typedef struct{
	char *t_name;
	int f_counter;
	FIELD **f;
	int reg_size;
	int num_elem;
	data dat;
}TAB;

typedef struct{
	char *index_field;
	char *index_table;
}tab_index;

/////////////////
typedef struct{
int ntables;
int nindexes;
int ninserts;
int nselects;
int nsorts;
int nshowtab;
int nshowindex;
int binlast;
int seqlast;
}stats;

stats empty_stats();

void bubble_sort_char (char **, int *, int);

void bubble_sort_double (double *, int *, int);

void bubble_sort_int (int *, int *, int);

void bubble_sort_float (float *, int *, int);

int binary_serach_char(char **, int *, int, int, char*);

int binary_serach_double (double *, int *, int, int, double);

int process_type(char *);

int process(char *);

void f_table(TAB **, int);

void f_data(tab_index *, int);

#endif