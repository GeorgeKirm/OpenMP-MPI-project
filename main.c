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

	struct timespec startTime, endTime;
	int i;
	int numberOfCoordinates = atoi(argv[2]);
	int counterForCorrectCoordinates = 0;
	
	FILE *readFile;
	char buffer[30];
	readFile = fopen(argv[3],"r");
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
	
	//reduction declares that the shared variable will be modified by all threads
	#pragma omp parallel for default(shared) private(i) reduction (+:counterForCorrectCoordinates)
	for (i = 0; i < numberOfCoordinates; i++){
        
		fgets(buffer,20,readFile);
		printf("%s",buffer);
		//if (correctCord)
		//counterForCorrectCoordinates++;
		
	}
	#pragma omp master
		clock_gettime(CLOCK_MONOTONIC, &endTime);
		printf("Prepei na typwnetai oso kai ta threads\n");
    }
    return 0;
}
