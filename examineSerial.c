#include "examineSerial.h"

/*
* char *argv[] parameters:
*  argv[1]: max limit to examine or -1 to mark no limit
*  argv[2]: max limit time to examine coordinates or -1 to mark no limit
*  argv[3]: File's name
*  argv[4]: Thread that are being used or -1 to mark no limit
*  argv[5]: Proccesses to use MPI or -1 to mark no limit
*/


float strToF(const char* s){
  float asd = 0, fact = 1;
  if (*s == '-'){
    s++;
    fact = -1;
  };
  int ch=0;
  for ( ch = 0; *s; s++){
    if (*s == '.'){
      ch = 1; 
      continue;
    };
    int d = *s - '0';
    if (d >= 0 && d <=9){
      if (ch) fact /= 10.0F;
      asd = asd * 10.0F + (float)d;
    };
  };
  return asd * fact;
}
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
	if(atof(argv[1])==0||atof(argv[2])==0||atof(argv[4])==0||atof(argv[5])==0){
		printf("Arguments cant be 0\n");
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
void checker(char *argv[]) {	FILE * pFile;
	long lSize, lSizeF;
	long lSizeL = 0;
	char * buffer;
	long bufferSize;
	int usableCoordinates = 0;	
	int breaker = 2;

	pFile = fopen ( argv[3] , "r" );
	if (pFile==NULL) {
		printf("File error");
		exit (1);
	}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSizeF = ftell (pFile);
	int coordinatesToExamine = atoi(argv[1]);
	if(coordinatesToExamine > 0) {
		if(coordinatesToExamine*30 < lSizeF) {
			lSizeF = coordinatesToExamine*30;
		}
	}
	//printf("%ld\n", lSizeF);
	rewind (pFile);
	if(lSizeF > MALLOC_SIZE*2) {
		lSize = MALLOC_SIZE;
	} else {
		lSize = lSizeF;
		breaker--;
	}

	do {
		bufferSize = lSize - lSizeL;
		// allocate memory to contain the whole file:

		buffer = (char*) malloc (sizeof(char)*bufferSize+1);
		if (buffer == NULL) {
			printf("Memory error");
			exit (2);
		}
		// copy the file into the buffer:
		bufferSize = fread (buffer, 1, bufferSize, pFile);
		buffer[bufferSize] = '\0';
		/* the whole file is now loaded in the memory buffer. */

		usableCoordinates = checkerSer(buffer, bufferSize) + usableCoordinates;
		free (buffer);
		lSizeL = lSize;
		lSize = lSize + MALLOC_SIZE;
		//printf("Loula: %d, %ld %ld\n", breaker, lSize, lSizeL);
		if( lSize >= lSizeF ) {
			lSize = lSizeF;
			breaker--;
		}
	} while(breaker > 0);
	fclose (pFile);

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
int checkerSer(char* buffer, size_t bufferSize)	{
	int usableCoordinates=0;
	//printf("%d\n",coordinateNumberToExamine);
	//char line[30]; /* or other suitable maximum line size */
		float distance=0;
		int a;
		for(a = 0; a < bufferSize/10; a=a+3 ){
				char nLine1[10];
				char nLine2[10];
				char nLine3[10];
				float value[3];
				strncpy(nLine1,buffer+(a*10),9);
				strncpy(nLine2,buffer+(a*10+10),9);
				strncpy(nLine3,buffer+(a*10+20),9);
				nLine1[9] = '\0';
				nLine2[9] = '\0';
				nLine3[9] = '\0';
				//value[0] = atof(nLine1);
				//value[1] = atof(nLine2);
				//value[2] = atof(nLine3);
				value[0] = strToF(nLine1);
				value[1] = strToF(nLine2);
				value[2] = strToF(nLine3);
									
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
