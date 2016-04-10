#!/bin/sh
#PBS -N testing
#PBS -j oe
#PBS -m n
#PBS -l nodes=7
#PBS -l walltime=00:15:00
export OMP_NUM_THREADS=12
echo "working direcory is $PWD"
cd /home/vamartid/Desktop/OpenMP-MPI-project-master
mpirun ./examineParallel.o -1 -1 datafile -1 -1
