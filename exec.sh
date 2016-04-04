#!/bin/sh
#PBS -N testing
#PBS -j oe
#PBS -m n
#PBS -l nodes=7
#PBS -l walltime=00:15:00
export OMP_NUM_THREADS=12
cd $HOME/Katanemmena
echo "working direcory is $PWD, at $HOSTNAME"
mpirun ./examineParallel.o -1 -1 datafile -1 -1
