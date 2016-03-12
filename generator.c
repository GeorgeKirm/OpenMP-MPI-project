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
	}
	fwrite(cords, sizeof(*cords),coordinate_index*3,file);
	fclose(file);
	/*
	printf("%lu\n",sizeof(float));
	printf("---palios pinakas--\n");
	printf("%f\n%f\n%f\n", cords[0][0],cords[0][1],cords[0][2]);
	//binary
	FILE *file2  = fopen(argv[1],"rb");
	fseek(file2, 0, SEEK_END);
	int lengthOfFile = ftell(file2);
	printf("PLITHOS %d",lengthOfFile/4/3/3 );
	
	float cords2[coordinate_index][3];
	printf("---meta to binary--\n");
	fread(cords2, sizeof(float),coordinate_index*3,file2);
	printf("%f\n%f\n%f\n", cords2[0][0],cords2[0][1],cords2[0][2]);
	fclose(file2);
	*/
	return 0;
}
