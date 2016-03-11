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
		exit(0);
	}

	struct timespec startTime, endTime;
	clock_gettime(CLOCK_MONOTONIC, &startTime);

	Array cords;
	readingFile(argv, &cords); // read datafile
	//printf("%f\n", cords.cord1[0]);
	checker(&cords); // Do the things




	// getting time of program
	clock_gettime(CLOCK_MONOTONIC, &endTime);
	const int DAS_NANO_SECONDS_IN_SEC = 1000000000;
	long timeElapsed_s = endTime.tv_sec - startTime.tv_sec;
	long timeElapsed_n = endTime.tv_nsec - startTime.tv_nsec;
	if(timeElapsed_n < 0) {
		timeElapsed_n = DAS_NANO_SECONDS_IN_SEC + timeElapsed_n;
		timeElapsed_s--;
	}
	printf("Time: %ld.%09ld secs \n", timeElapsed_s, timeElapsed_n);

	return 0;
}

void checker(Array *cords)	{
	float distance=0;
	int usableCoordinates=0;
	int a;
	#pragma omp parallel
	{
		#pragma omp for
		for(a = 0; a < cords->used; a++ ){
			if(a<=10){
				printf("Hello world from thread %d\n", omp_get_thread_num());
			}
			float k = cords->cord1[a] * cords->cord1[a] + cords->cord2[a] * cords->cord2[a] + cords->cord3[a] * cords->cord3[a];
			distance= sqrtf( k );
			if( (distance>=12) && (distance<=30) ){
				usableCoordinates++;
			}
			distance=0;
		}
	}
		/* 
		*	//testing the threads
		*  tid = omp_get_thread_num();
		*	printf("Hello world from thread %d\n", tid);
		*/
	
	printf("Number of usable cordinates = %d\n", usableCoordinates);
}

void readingFile(char *argv[], Array *cords){
	FILE *readFile;
	readFile = fopen(argv[3],"r");
	if(readFile==NULL) { //exit file if there is no data file
		printf("Error while opening the file. Run generator.o\n");
		exit(0);
	}
	int coordinateNumberToExamine= atoi(argv[1]);
	//Array cords;
	if(coordinateNumberToExamine==-1){
		initArray(cords, 1000);
	} else {
		initArray(cords, coordinateNumberToExamine);
	}	
	char line[256]; /* or other suitable maximum line size */
	int stoper=0;
	while (fgets(line, sizeof line, readFile) != NULL && stoper!= coordinateNumberToExamine) {
		float value1 = atof(line);
		fgets(line, sizeof line, readFile);
		float value2 = atof(line);
		fgets(line, sizeof line, readFile);
		float value3 = atof(line);
		//printf("%f %f %f\n", value1, value2, value3);
		insertArray(cords, value1, value2, value3);
		stoper++;
	}
	//printf("%f\n", cords->cord1[0]);
	fclose(readFile);
}

void initArray(Array *a, int initialSize) {
	a->cord1 = (float *)malloc(initialSize * sizeof(float));
	a->cord2 = (float *)malloc(initialSize * sizeof(float));
	a->cord3 = (float *)malloc(initialSize * sizeof(float));
	a->used = 0;
	a->size = initialSize;
}

void insertArray(Array *a, float value1, float value2, float value3) {
	if (a->used == a->size) {
		a->size *= 2;
		a->cord1 = (float *)realloc(a->cord1, a->size * sizeof(float));
		a->cord2 = (float *)realloc(a->cord2, a->size * sizeof(float));
		a->cord3 = (float *)realloc(a->cord3, a->size * sizeof(float));
	}
	a->cord1[a->used++] = value1;
	a->cord2[a->used] = value2;
	a->cord3[a->used] = value3;
}

void freeArray(Array *a) {
	free(a->cord1);
	free(a->cord2);
	free(a->cord3);
	a->cord1 = NULL;
	a->cord2 = NULL;
	a->cord3 = NULL;
	a->used = a->size = 0;
}

