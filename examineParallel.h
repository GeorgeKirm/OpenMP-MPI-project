#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#define DOWNLIMIT 12
#define UPLIMIT 30

#define DEBUG 1
#define DEBUG1 1
#define DEBUG2 1

void printTime(struct timespec,struct timespec);

void checkerOMP(char**, char*, size_t);

void numberOfThreads(int);
