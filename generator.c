#include "generator.h"

int main(int argc, char *argv[]) {
	/*
	Check if the given arguments are more than 1. 
	*/
	if(argc!=3){
		printf("Wrong number of arguents. Exiting program");
		return 0;
	}
	//printf("%s %s %d\n",argv[1],argv[2],atoi(argv[2]));
	int coordinate_index = atoi(argv[2]);
	int utime;
	long int ltime;
	int i;
	float cords[coordinate_index][3];
	ltime = time(NULL);
	utime = (unsigned int) ltime/2;
	FILE *file  = fopen(argv[1],"w+");

	srand(utime);
	for(i=0;i<coordinate_index;i++){
		cords[i][0]=(float)34*rand()/(RAND_MAX-1);
		cords[i][1]=(float)34*rand()/(RAND_MAX-1);
		cords[i][2]=(float)34*rand()/(RAND_MAX-1);
		//fprintf(file,"%f %f %f\n", cords[i][0],cords[i][1],cords[i][2]);    
		fprintf(file,"%f\n%f\n%f\n", cords[i][0],cords[i][1],cords[i][2]); 
	}
	return 0;
}
