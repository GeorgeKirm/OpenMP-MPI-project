#include "examineSerial.h"

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
	// getting the start time of program
	struct timespec startTime, endTime;
	clock_gettime(CLOCK_MONOTONIC, &startTime);

	checker(argv);

	// getting the end time of program
	clock_gettime(CLOCK_MONOTONIC, &endTime);
	printTime(startTime,endTime);

	return 0;
}

void printTime(struct timespec a,struct timespec b) {
	const int DAS_NANO_SECONDS_IN_SEC = 1000000000;
	long timeElapsed_s = b.tv_sec - a.tv_sec;
	long timeElapsed_n = b.tv_nsec - a.tv_nsec;
	if(timeElapsed_n < 0) {
		timeElapsed_n = DAS_NANO_SECONDS_IN_SEC + timeElapsed_n;
		timeElapsed_s--;
	}
	printf("Time: %ld.%09ld secs \n", timeElapsed_s, timeElapsed_n);
}

void checker(char *argv[]) {
	// reading the values from the file
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen ( argv[3] , "r" );
	if (pFile==NULL) {
		printf("File error");
		exit (1);
	}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {
		printf("Memory error");
		exit (2);
	}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {
		printf("Reading error");
		exit (3);
	}
	/* the whole file is now loaded in the memory buffer. */
	fclose (pFile);
	
	int usableCoordinates = checkerSer(argv, buffer, result);
	free (buffer);

	printf("Number of usable cordinates = %d\n", usableCoordinates);
}

int checkerSer(char *argv[], char* buffer, size_t bufferSize)	{
	float distance=0;
	int usableCoordinates=0;
	int a;
	int coordinateNumberToExamine = atoi(argv[1]);
	if(coordinateNumberToExamine >= 0){
		// coordinateNumberToExamine = atoi(argv[1]);
	} else {
		coordinateNumberToExamine = bufferSize/30;
	}
	//printf("%d\n",coordinateNumberToExamine);
	//char line[30]; /* or other suitable maximum line size */
	for(a = 0; a < coordinateNumberToExamine*3; a=a+3 ){
		char nLine1[10];
		char nLine2[10];
		char nLine3[10];
		float value[3];
		strncpy(nLine1,buffer+(a*10),9);
		strncpy(nLine2,buffer+(a*10+10),9);
		strncpy(nLine3,buffer+(a*10+20),9);
		nLine1[10] = '\0';
		nLine2[10] = '\0';
		nLine3[10] = '\0';
		value[0] = atof(nLine1);
		value[1] = atof(nLine2);
		value[2] = atof(nLine3);
		/*/
		printf("%f %f %f\n", value[0], value[1], value[2]);
		printf("2. %f\n",value[1]);
		printf("3. %f\n",value[2]);
		printf("L. %f\n",value[0]*value[0] + value[1]*value[1] + value[2]*value[2]);
		//*/
		distance= sqrtf(value[0]*value[0] 
				+ value[1]*value[1] + value[2]*value[2]);
		if((distance>=DOWNLIMIT) && (distance<=UPLIMIT)) {
			usableCoordinates++;
		}
		distance=0;
	}
	//printf("Number of usable cordinates = %d\n", usableCoordinates);
	return usableCoordinates;
}
