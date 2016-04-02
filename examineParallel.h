#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#define DOWNLIMIT 12
#define UPLIMIT 30

#define DEBUG 0
#define DEBUG1 0
#define DEBUG2 0

void printTime(struct timespec,struct timespec);

void checker(char**);

int checkerOMP(char**, char*, size_t);

void numberOfThreads(int);
