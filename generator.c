#include "generator.h"
#include "defineBinary.h"

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
	if(BINARY==0) {
		for(i=0;i<coordinate_index;i++){
			fprintf(file,"%9.6f\n%9.6f\n%9.6f\n", (float)34*rand()/(RAND_MAX-1),(float)34*rand()/(RAND_MAX-1),(float)34*rand()/(RAND_MAX-1));
		}
	} else {
		for(i=0;i<coordinate_index;i++){
			cords[i][0]=(float)34*rand()/(RAND_MAX-1);
			cords[i][1]=(float)34*rand()/(RAND_MAX-1);
			cords[i][2]=(float)34*rand()/(RAND_MAX-1);
		}
		fwrite(cords, sizeof(*cords),coordinate_index*3,file);
	}	
	fclose(file);
	return 0;
}
