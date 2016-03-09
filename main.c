#include <omp.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    int tid;
	FILE *readFile;
	readFile = fopen(argv[1],"r");

	int numberOfCoordinates = atoi(argv[2]);
	if (readFile == NULL)
	{
		perror("Error while opening the file");
	}
	/*
		na diavazei to arxeio kai epeita na vgazei ta pososta
	*/
	for(int i = 0; i < numberOfCoordinates;i++)
	{
		
	}

    #pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();
        printf("Hello world from thread %d\n", tid);
    }
    return 0;
}
