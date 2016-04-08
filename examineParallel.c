#include "examineParallel.h"

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

	int rank = checker(argv);

	// getting the end time of program
	if (rank == 0) {
		clock_gettime(CLOCK_MONOTONIC, &endTime);
		printTime(startTime,endTime);
	}
	return 0;
}

void printTime(struct timespec a,struct timespec b){
	const int DAS_NANO_SECONDS_IN_SEC = 1000000000;
	long timeElapsed_s = b.tv_sec - a.tv_sec;
	long timeElapsed_n = b.tv_nsec - a.tv_nsec;
	if(timeElapsed_n < 0) {
		timeElapsed_n = DAS_NANO_SECONDS_IN_SEC + timeElapsed_n;
		timeElapsed_s--;
	}
	printf("Time: %ld.%09ld secs \n", timeElapsed_s, timeElapsed_n);
}

void numberOfThreads(int threadsLimit) {
	if(threadsLimit>0) {
		omp_set_num_threads(threadsLimit);
	} else {
		//nothing to do
	}
}

int checker(char *argv[])	{
	int processLimit = atoi(argv[5]);
	int rank,size;
	int error;
	int sum = 0;

	/*
		Abort if initialize failed.
	*/
	error = MPI_Init(NULL, NULL);
	if(error != MPI_SUCCESS) {
		MPI_Abort(MPI_COMM_WORLD,error);
	}
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

        if((size < processLimit) || (processLimit == -1)) {
                //size == all thread
        } else {
                size = processLimit;
        }
	int coordinatesToExamine = atoi(argv[1]);
	if(coordinatesToExamine > 0) {
		if(coordinatesToExamine < size) {
			size = coordinatesToExamine;
		}
	}
        if(rank < size) {
	/*
		vvv Start of reading file vvv
	*/
		// reading the values from the file
		FILE * pFile;
		long lSize, lSizeF;
		long lSizeL = 0;
		char * buffer;
		size_t bufferSize;
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
		rewind (pFile);
		
		if(lSizeF > MALLOC_SIZE*2) {
			lSize = MALLOC_SIZE;
		} else {
			lSize = lSizeF;
			breaker--;
		}

		do {
			
			bufferSize = lSize - lSizeL;
        	        bufferSize = bufferSize/(3*BITS_LINE);
               		size_t startToRead = (bufferSize % size) ? bufferSize/size + 1 : bufferSize/size;
               		size_t endToRead = startToRead;
			startToRead = startToRead*rank*(3*BITS_LINE);
               	 	endToRead = endToRead*(rank+1)*(3*BITS_LINE);
			if(processLimit == -1){
                	        if(rank == size-1) {
                        	        endToRead = bufferSize*(3*BITS_LINE);
                        	}
                	} else {
                        	if(rank == processLimit) {
                                	endToRead = bufferSize*(3*BITS_LINE);
                        	}
                	}
							
			// allocate memory to contain the whole file
			//printf("vvv\n%d: %zu %zu\n^^^\n", rank, startToRead, endToRead);
        		buffer = (char*) malloc (sizeof(char)*(endToRead-startToRead)+1);
			if (buffer == NULL) {
				printf("Memory error");
				exit (2);
			}

			// copy the file into the buffer:
			fseek(pFile ,startToRead+lSizeL , SEEK_SET);
			bufferSize = fread (buffer, 1, endToRead - startToRead, pFile);
			buffer[endToRead-startToRead] = '\0';
//			printf("\nVVVVV\n%d: %s\n^^^^^\n", rank, buffer);

//			fclose (pFile);
		/* the whole file is now loaded in the memory buffer. */

		/*
			^^^ End of reading file ^^^
		*/

			#if DEBUG1
				printf("rank %d\n",rank);
				printf("size %d\n",size);
				char name[100];
				int len;
				MPI_Get_processor_name(name, &len);
				printf("Processor name is: %s\n",name);
			#endif

			numberOfThreads(atoi(argv[4]));
			usableCoordinates = checkerOMP(rank, buffer, bufferSize) + usableCoordinates;
			//printf("usableCoordinates in rank %d: %d\n",rank, usableCoordinates);
			free (buffer);
			lSizeL = lSize;
			lSize = lSize + MALLOC_SIZE;
			if( lSize > lSizeF ) {
				lSize = lSizeF;
				breaker--;
			}
			//printf("\nLOULA\n\n");
		} while(breaker > 0);

		MPI_Reduce(&usableCoordinates,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);


	} else { // BALE ERROR MESSAGE
	}
//	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	if(rank == 0) {
		printf("\nNumber of usable cordinates = %d\n", sum);
	}
	return rank;
}

int checkerOMP(int rank, char* buffer, size_t bufferSize)	{
//	printf("%zu, %zu\n", startToread, endToRead);
	int usableCoordinates = 0;
	#pragma omp parallel
	{
		float distance=0;
		int a;
		int temp=0;
		//printf("vvv\n%s\n%zu %zu\n^^^\n", buffer, startToread/BITS_LINE, endToRead/BITS_LINE);
//		printf("vvv\n%zu %zu\n^^^\n", startToread/BITS_LINE, endToRead/BITS_LINE);
		#pragma omp for schedule(static,1) private(a, distance)
		for(a = 0; a < bufferSize/10; a = a+3) {
//		for(a = startToread/BITS_LINE; a < endToRead/BITS_LINE; a=a+3 ){
			char nLine1[BITS_LINE];
			char nLine2[BITS_LINE];
			char nLine3[BITS_LINE];
			float value[3];
			strncpy(nLine1,buffer+(a*BITS_LINE+(BITS_LINE*0)),BITS_LINE-1);
			strncpy(nLine2,buffer+(a*BITS_LINE+(BITS_LINE*1)),BITS_LINE-1);
			strncpy(nLine3,buffer+(a*BITS_LINE+(BITS_LINE*2)),BITS_LINE-1);
			nLine1[BITS_LINE] = '\0';
			nLine2[BITS_LINE] = '\0';
			nLine3[BITS_LINE] = '\0';
			value[0] = atof(nLine1);
			value[1] = atof(nLine2);
			value[2] = atof(nLine3);
			/*/
			printf("%d: %f %f %f\n", rank, value[0], value[1], value[2]);
			//printf("%f\n", value[0]*value[0] + value[1]*value[1] + value[2]*value[2]);
			// */
			distance= sqrtf(value[0]*value[0] 
					+ value[1]*value[1] + value[2]*value[2]);
			if((distance>=DOWNLIMIT) && (distance<=UPLIMIT)) {
				temp++;
			}
			distance=0;
		}
		//printf("%d: %d\n", omp_get_thread_num(), temp);
		#pragma omp critical
			usableCoordinates = usableCoordinates+temp;
	}
	//printf("Number of usable cordinates = %d\n", usableCoordinates);
	return usableCoordinates;
}
