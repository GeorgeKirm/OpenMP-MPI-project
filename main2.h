#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DOWNLIMIT 12
#define UPLIMIT 30


typedef struct {
	float *cord1;
	float *cord2;
	float *cord3;
	//size_t used;
	//size_t size;
	int used;
	int size;
} Array;

void initArray(Array *, int);

void insertArray(Array *, float, float, float);

void freeArray(Array *);
