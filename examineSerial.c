#include "examineSerial.h"

/*
* char *argv[] parameters:
*  argv[1]: max limit to examine or -1 to mark no limit
*  argv[2]: max limit time to examine coordinates or -1 to mark no limit
*  argv[3]: File's name
*  argv[4]: Thread that are being used or -1 to mark no limit
*  argv[5]: Proccesses to use MPI or -1 to mark no limit
*/

/**main
 *
 *This function is the main which calls the other to work
 *has 2 variables for input argc and argv[]
 *argc is the number of the argv cells
 *argv values are explained above
 *So main checks the number of argv values and if its ok it
 *start working.Specificaly it start's the clock
 *it calls checker function using the input 
 *file (created  with the generator)
 *and it end's the time after that(calls printTime function).
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

/**printTime
 *
 *print time use's two timespec struct variables
 *whitch contains a value that express the time of
 *the system and by doing the right maths
 *it calculates the time between the two timespecs in seconds
 *It is used by main in order to print the time the program was running
 */
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

/**
 *checker
 * opens the data file
 * find it's size and then goes back to start
 * it reads it using the buffer and prints the errors if there are any
 * then it close the file
 * and then calls the checkerSer in order to find the usable coordinates
 * when checkerSer ends it prints the number of the usable coordinates
 */
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
/**
 *checkerSer
 *checks the limit of the cordinates
 *checks the limit of the time
 *starts the clock and then goes to the for
 *it takes 3 lines each time in order to check the coordinates
 *it check if there are in the space we need
 *then it stops the clock
 *and checks if the time limit is exided
 *if yes it just itterate to the end
 *if not it continues till the end
 *then it returns the usable coordinates
 */
int checkerSer(char *argv[], char* buffer, size_t bufferSize)	{
	int coordinateNumberToExamine = atoi(argv[1]);
	int usableCoordinates=0;
	if(coordinateNumberToExamine >= 0){
		// coordinateNumberToExamine = atoi(argv[1]);
	} else {
		coordinateNumberToExamine = bufferSize/30;
	}
	//printf("%d\n",coordinateNumberToExamine);
	//char line[30]; /* or other suitable maximum line size */
	double time_limit = strtod(argv[2],NULL);
	if((time_limit>0)||(time_limit==-1)){
		float distance=0;
		int a;
		clock_t tstart;	
		tstart = clock();
		double tend=0;
		int flag=0;
		tend =0;
		for(a = 0; a < coordinateNumberToExamine*3; a=a+3 ){
				if(flag==1) continue;			
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
				tend = (double)(clock() - tstart) / CLOCKS_PER_SEC;
				if ((time_limit>0)&&(tend>0)&&(tend >= time_limit)){
					flag=1;
				}
		}	
	}
	//printf("Number of usable cordinates = %d\n", usableCoordinates);
	return usableCoordinates;
}
