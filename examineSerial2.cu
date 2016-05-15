//#include "examineSerial2.h"
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define DOWNLIMIT 12
#define UPLIMIT 30
#define LAST_RANK 6
#define BITS_LINE 10
#define MALLOC_SIZE 240000 //80000 sugkrouseis
//#define KERNEL 512


void printTime(struct timespec,struct timespec);

void checker(char**);


__device__ float strToF(const char* s){
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
    if (d >= 0 && d <= 9){
      if (ch) fact /= 10.0F;
      asd = asd * 10.0F + (float)d;
    };
  };
  return asd * fact;
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
__global__ void checkerSer(char* buffer, long * bufferSize, int * usableCoordinates)	{

	//printf("%d\n",coordinateNumberToExamine);
	//char line[30]; /* or other suitable maximum line size */
	float distance=0;
	//int a;

	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	bufferSize[0] = bufferSize[0]/10;
	if(idx<bufferSize[0]/3) {
//		for(a = 0; a < bufferSize/10; a=a+3 ) {		
		char nLine1[BITS_LINE];
		char nLine2[BITS_LINE];
		char nLine3[BITS_LINE];
		float value[3];
		int b;
		for(b=0 ; b < (10); b++) {
			nLine1[b] = buffer[idx*3*10+(10*0)+b];
			nLine2[b] = buffer[idx*3*10+(10*1)+b];
			nLine3[b] = buffer[idx*3*10+(10*2)+b];			
		}
		nLine1[9] = '\0';
		nLine2[9] = '\0';
		nLine3[9] = '\0';
		value[0] = strToF(nLine1);
		value[1] = strToF(nLine2);
		value[2] = strToF(nLine3);
		distance= sqrtf(value[0]*value[0] + value[1]*value[1] + value[2]*value[2]);
		if((distance>=DOWNLIMIT) && (distance<=UPLIMIT)) {
//			__syncthreads();
			//printf("%f %f %f",value[0],value[1],value[2]);
			atomicAdd(usableCoordinates,1);
			//(*usableCoordinates)++;
			//(*uc)++;
		}
//__syncthreads();
		//distance=0;
	}
}


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
void checker(char *argv[]) {

	/*
	int coordinatesToExamine = atoi(argv[1]);
	if(coordinatesToExamine > 0) {
		if(coordinatesToExamine < size) {
			size = coordinatesToExamine;
		}
	}*/

	// reading the values from the file
	FILE * pFile;
	long lSize, lSizeF;
	long lSizeL = 0;
	char * buffer;
	long bufferSize;
	int usableCoordinates = 0;	
	int breaker = 2;
        //int  h_usableCoordinates = 0;  

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
	if(lSizeF > MALLOC_SIZE) {
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

//######################
		char * d_buffer;
		long * d_bufferSize;
		long * h_bufferSize;
		int * d_usableCoordinates;
		h_bufferSize = &bufferSize;
		//h_usableCoordinates = (int *)malloc(sizeof(int));        // Allocate array on host
		if(cudaMalloc((void**)&d_buffer, bufferSize) != cudaSuccess) {
			printf("cudaMalloc error 1\n");
			exit(4);
		}
		if(cudaMalloc((void**)&d_bufferSize, sizeof(long)) != cudaSuccess) {
			printf("cudaMalloc error 2\n");
			free(d_buffer);
			exit(4);
		}
		if(cudaMalloc((void**)&d_usableCoordinates, sizeof(int)) != cudaSuccess) {
			printf("cudaMalloc error 3\n");
			free(d_buffer);
			free(d_bufferSize);
			exit(4);
		}
		if(cudaMemcpy(d_buffer,buffer, bufferSize, cudaMemcpyHostToDevice) != cudaSuccess) {
			printf("cudaMemcpy error 4\n");
			cudaFree(d_buffer);
			cudaFree(d_bufferSize);
			cudaFree(d_usableCoordinates);
			exit(5);
		}
		if(cudaMemcpy(d_bufferSize,h_bufferSize, sizeof(long), cudaMemcpyHostToDevice) != cudaSuccess) {
			printf("cudaMemcpy error 5\n");
			cudaFree(d_buffer);
			cudaFree(d_bufferSize);
			cudaFree(d_usableCoordinates);
			exit(5);
		}
		if(cudaMemcpy(d_usableCoordinates,&usableCoordinates,sizeof(int),cudaMemcpyHostToDevice)!=cudaSuccess){
			printf("cudaMemcpy error 7\n");
			cudaFree(d_buffer);
			cudaFree(d_bufferSize); 
                        cudaFree(d_usableCoordinates);
			exit(5);
		}

//*****************Kernel Run*******************************//
		//checkerSer<<<bufferSize/(30*KERNEL) + 1, KERNEL>>>(d_buffer, d_bufferSize, d_usableCoordinates);
		long loula = bufferSize/30;
		int KERNEL = 512;
		if (loula < KERNEL) {
			KERNEL = loula;
		}		
		dim3 grid(loula/KERNEL + (loula%KERNEL == 0 ? 0:1));            // defines a grid of 256 x 1 x 1 blocks
		dim3 block(KERNEL);       // defines a block of 512 x 512 x 1 threads
		checkerSer<<<grid, block>>>(d_buffer, d_bufferSize, d_usableCoordinates);
	//	checkerSer<<<1,512>>>(d_buffer, d_bufferSize, d_usableCoordinates);
		if(cudaMemcpy(&usableCoordinates,d_usableCoordinates, sizeof(int), cudaMemcpyDeviceToHost) != cudaSuccess) {
			printf("cudaMemcpy error 6\n");
			cudaFree(d_buffer);
			cudaFree(d_bufferSize);
			cudaFree(d_usableCoordinates);
			exit(5);
		}
//######################
//printf("%d \n", usableCoordinates);
//		h_usableCoordinates = usableCoordinates + h_usableCoordinates;
//printf("Line 290\n");		

		cudaFree(d_buffer);
		cudaFree(d_bufferSize);
		cudaFree(d_usableCoordinates);
//usableCoordinates = *d_usableCoordinates;
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
//	printf("add the numbers number %d \n",h_usableCoordinates);
	printf("Number of usable cordinates = %d\n", usableCoordinates);
}
