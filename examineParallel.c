#include "examineParallel.h"

/**
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

/**printTime
 *
 *print time use's two timespec struct variables
 *whitch contains a value that express the time of
 *the system and by doing the right maths
 *it calculates the time between the two timespecs in seconds
 *It is used by main in order to print the time the program was running
 */
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

/**numberOfThreads
 *
 *numberOfThreads use's an int  variable called threadsLimit
 *If it is bigger than 0 it specifies the number of 
 *threads used in subsequent parallel sections if it's lower then it does nothing
 *and let it in the default value.
 *It is used by checker in order to give set the number of threads according to argv[4]
 */
void numberOfThreads(int threadsLimit) {
	if(threadsLimit>0) {
		omp_set_num_threads(threadsLimit);
	} else {
		//nothing to do
	}
}

/**checker
 *
 *checker use's the argv variable
 *Initializes the MPI execution environment and check's if it was successfull
 *Determines the rank of the calling process in the communicator
 *and the size of the group associated with him
 *size is the Number of the ranks that will be used from the cluster
 *It changes the size variable to the proccess limit if it is bigger and 
 *not the deafault
 *It changes the size variable to the coordinatesToExamine 
 *if it NOT not the deafault AND it is smaller than the size(one coordinate check per
 * group)
 *If the rank-size is ok then it start's reading
 *It opens the the file and prints error if not
 *by using fseek() we add offset to the end of
 *the file in order to find the file's SIZE.
 *With fTell() after we take the number of bytes from the beginning of the file.
 *It sets the file position to the beginning of the file of the INPUT stream with rewind()
 *We check if we have more than 200.000 Coordinates to examine (3000000*2/3=...)then we
 *split the size and malocs else if we have lower we do one maloc only (breacker--)
 *With MPI_Reduce we reduce values on all processes to a single value
 *After It Terminates MPI execution environment 
 *Finaly it prints the usable coordinates from the coordinates given in the input
 *checker method is being used by main method with the argv variable
 */
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
	//update_string(argv[2]);
	//time_limit = atof(argv[2]);
//	double time_limit = strtod(argv[2],NULL);
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
               		//size_t startToRead = (bufferSize % size) ? bufferSize/size + 1 : bufferSize/size;
               		size_t startToRead = bufferSize/size;
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
			//usableCoordinates = checkerOMP(rank, buffer, bufferSize,&time_limit) + usableCoordinates;
			usableCoordinates = checkerOMP(rank, buffer, bufferSize) + usableCoordinates;
			//printf("usableCoordinates in rank %d: %d\n",rank, usableCoordinates);
			free (buffer);
			lSizeL = lSize;
			lSize = lSize + MALLOC_SIZE;
			if( lSize > lSizeF ) {
				lSize = lSizeF;
				breaker--;
			}
		} while((breaker > 0));
//		} while((breaker > 0)&&!(time_limit=-1));

		MPI_Reduce(&usableCoordinates,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);


	}//fige sto finalize
//	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	if(rank == 0) {
		printf("\nNumber of usable cordinates = %d\n", sum);
	}
	return rank;
}

/**checkerOMP
 *
 *checkerOMP use's the rank the buffer and the bufferSize
 *intialze the start time of the for
 *start the omp parallel with 2 shared variable (in order the threads see the same times)
 *now intializes a,distance and temp the  parallel region 
 *Start's the loop construct specifies that the iterations of loops will be distributed 
 *among and executed by the encountering team of threads.
 *In the loop:
 *Calculates the time in each for and choose eather to keep doing the code or not to continue and
 *just itterate through the for.
 *Now in the for we take one line and the 2 after lines in order to have the first cordinates(used buffer here)
 *to examine if it is in the space we want.
 *when the  loop is finished we return the usable Coordinates
 *Now if the loop has over used the time limit then it enables the flag it iterates for the next for i++ times
 * and it passes through the POINTER and Memory address to the checker function the time_limit change to '-1'
 *this way we can stop checkers while from continue calling the for and stop the certain time the user wanted
 *used from checker function in order to split the proccessing of the data to the threads
 */
//int checkerOMP(int rank, char* buffer, size_t bufferSize,double *time_limit)	{
int checkerOMP(int rank, char* buffer, size_t bufferSize)	{
//	printf("%zu, %zu\n", startToread, endToRead);
	int usableCoordinates=0;
	//if((*time_limit>0)||(*time_limit==-1)){
	//	double tstart = omp_get_wtime();
		//int flag=0;
		//double tend =0;
		#pragma omp parallel //shared(flag,tend)
		{
			float distance=0;
			int a;
			int temp=0;
			//printf("vvv\n%s\n%zu %zu\n^^^\n", buffer, startToread/BITS_LINE, endToRead/BITS_LINE);
	//		printf("vvv\n%zu %zu\n^^^\n", startToread/BITS_LINE, endToRead/BITS_LINE);
			#pragma omp for schedule(static,1) private(a, distance)
			for(a = 0; a < bufferSize/10; a = a+3) {
	//		for(a = startToread/BITS_LINE; a < endToRead/BITS_LINE; a=a+3 ){
				//if(flag==1) continue;
				char nLine1[BITS_LINE];
				char nLine2[BITS_LINE];
				char nLine3[BITS_LINE];
				float value[3];
				strncpy(nLine1,buffer+(a*BITS_LINE+(BITS_LINE*0)),BITS_LINE-1);
				strncpy(nLine2,buffer+(a*BITS_LINE+(BITS_LINE*1)),BITS_LINE-1);
				strncpy(nLine3,buffer+(a*BITS_LINE+(BITS_LINE*2)),BITS_LINE-1);
				nLine1[BITS_LINE-1] = '\0';
				nLine2[BITS_LINE-1] = '\0';
				nLine3[BITS_LINE-1] = '\0';
				value[0] = atof(nLine1);
				value[1] = atof(nLine2);
				value[2] = atof(nLine3);
				/*/
				printf("%d: %f %f %f\n", rank, value[0], value[1], value[2]);
				//printf("%f\n", value[0]*value[0] + value[1]*value[1] + value[2]*value[2]);
				// */
				distance= sqrtf(value[0]*value[0] + value[1]*value[1] + value[2]*value[2]);
				if((distance>=DOWNLIMIT) && (distance<=UPLIMIT)) {
					temp++;
				}
				distance=0;
				/*
				tend = (double)(omp_get_wtime() - tstart);
				if ((*time_limit>0)&&(tend>0)&&(tend >= *time_limit)){
					flag=1;
					*time_limit=-1;
				}*/
			}
			//printf("%d: %d\n", omp_get_thread_num(), temp);
			#pragma omp critical
				usableCoordinates = usableCoordinates+temp;
		}
		return usableCoordinates;		
	//}
	//printf("Number of usable cordinates = %d\n", usableCoordinates);
	return usableCoordinates;
}
