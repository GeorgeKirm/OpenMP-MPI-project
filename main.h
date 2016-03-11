#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DOWNLIMIT 12
#define UPLIMIT 30


void initArray(Array *, size_t);

void insertArray(Array *, int);

void freeArray(Array *);
