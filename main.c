#include <omp.h>
#include <stdio.h>
#include <time.h>
#define DOWNLIMIT 12
#define UPLIMIT 30

/*
*
* char *argv[] parameters:
*  argv[1]: max limit to examine or -1 to mark no limit
*  argv[2]: max limit time to examine coordinates or -1 to mark no limit
*  argv[3]: File's name
*  argv[4]: Thread that used or -1 to mark no limit
*  argv[5]: Proccesses to use MPI or -1 to mark no limit
*
*/
int main (int argc, char *argv[]) {
    int tid;
	FILE *readFile;
	readFile = fopen(argv[1],"r");
	struct timespec startTime, endTime;
	int i;
	int numberOfCoordinates = atoi(argv[2]);
	
	if (readFile == NULL)
	{
		perror("Error while opening the file");
	}
	//Na doume an o xronos einai topothetimenos swsta
	//mporei na xreiazetai na to valoume mesa sto crisimo tmima
    #pragma omp parallel private(tid)
    {
	#pragma omp master
	clock_gettime(CLOCK_MONOTONIC, &startTime);
	
	for (i = 0; i < numberOfCoordinates; i++){
        	tid = omp_get_thread_num();
        	printf("Hello world from thread %d\n", tid);
	}
		#pragma omp master
		{
		clock_gettime(CLOCK_MONOTONIC, &endTime);
		printf("Tasos sad\n");
		}
    }
    return 0;
}
