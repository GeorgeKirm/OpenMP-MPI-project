#include "main.h"

/*
* char *argv[] parameters:
*  argv[1]: max limit to examine or -1 to mark no limit
*  argv[2]: max limit time to examine coordinates or -1 to mark no limit
*  argv[3]: File's name
*  argv[4]: Thread that are being used or -1 to mark no limit
*  argv[5]: Proccesses to use MPI or -1 to mark no limit
*/
int main (int argc, char *argv[]) {
	if(argc!=6) {
		printf("Wrong number of arguents");
	}
	int tid; //thread id
	FILE *readFile;
	readFile = fopen(argv[1],"r");
	if(readFile==NULL) { //exit file if there is no data file
		printf("Error while opening the file. Run generator.o");
		exit(0);
	}
	int coordinateNumberToExamine= atoi(argv[1]);
	if(coordinateNumberToExamine==-1){
		float cords[1000][3]
	} else {
		float cords[coordinateNumberToExamine][3];
	}
	struct timespec startTime, endTime; // <- what is this???
	int i;
	int freeLine=0;
	int numberOfCoordinates = atoi(argv[2]);
	//Na doume an o xronos einai topothetimenos swsta
	//mporei na xreiazetai na to valoume mesa sto crisimo tmima
	#pragma omp parallel private(tid)
	{
		/* 
		*	//testing the threads
		*	tid = omp_get_thread_num();
		*	printf("Hello world from thread %d\n", tid);
		*/

		#pragma omp master
		{
			clock_gettime(CLOCK_MONOTONIC, &startTime);
		}


		#pragma omp master
		{
			char line[256]; /* or other suitable maximum line size */
			while (fgets(line, sizeof line, file) != NULL) /* read a line */ {
				float cords[coordinate_index][3];
				#pragma omp critical
				{

				}
			}
			fclose(file);
		}
		#pragma omp master
		{
			clock_gettime(CLOCK_MONOTONIC, &endTime);
			printf("Tasos sad\n");
		}
	}
	return 0;
}
