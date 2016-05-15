#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define DOWNLIMIT 12
#define UPLIMIT 30
#define LAST_RANK 6
#define BITS_LINE 10
#define MALLOC_SIZE 3000000 //100000 sugkrouseis

void printTime(struct timespec,struct timespec);

void checker(char**);

int checkerSer(char*, size_t);
