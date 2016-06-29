/*
 * generator.c
 * This file is part of OpenMP-MPI-project
 *
 * Copyright (C) 2016 - x
 *
 * OpenMP-MPI-project is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * OpenMP-MPI-project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenMP-MPI-project. If not, see <http://www.gnu.org/licenses/>.
 */

#include "generator.h"

int main(int argc, char *argv[]) {
	/*
	Check if the given arguments are more than 1. 
	*/
	if(argc!=3){
		printf("Wrong number of arguents. Exiting program");
		return 0;
	}
	int coordinate_index = atoi(argv[2]);
	int utime;
	long int ltime;
	int i;
	ltime = time(NULL);
	utime = (unsigned int) ltime/2;
	FILE *file  = fopen(argv[1],"w+");
	srand(utime);
	for(i=0;i<coordinate_index;i++){
		fprintf(file,"%9.6f\n%9.6f\n%9.6f\n", (float)34*rand()/(RAND_MAX-1),(float)34*rand()/(RAND_MAX-1),(float)34*rand()/(RAND_MAX-1));
	}
	fclose(file);
	return 0;
}
