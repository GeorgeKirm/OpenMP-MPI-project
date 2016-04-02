CC = gcc
MC = mpicc
CFLAGS = -Wall -g
LIBS = -L/path/to/libs -lm

# fast command
easy: buildGenerate buildExamineParallel runGenerate  runExamineParallel

# BUILD COMMANDS
# vvvvvvvvvvvvvv

all: buildGenerate buildExamineParallel buildExamineSerial

buildG: buildGenerate

buildE: buildExamineParallel

buildS: buildExamineSerial

buildExamineParallel: examineParallel.c examineParallel.h
	$(MC) $(CFLAGS) -fopenmp examineParallel.c examineParallel.h -o examineParallel.o $(LIBS)

buildExamineSerial: examineSerial.c examineSerial.h
	$(CC) $(CFLAGS) examineSerial.c examineSerial.h -o examineSerial.o $(LIBS)

buildGenerate: generator.c generator.h
	$(CC) $(CFLAGS) generator.c generator.h -o generator.o

# RUN COMMANDS
# vvvvvvvvvvvv

run: runGenerate runExamineParallel runExamineSerial

runG: runGenerate

runE: runExamineParallel

runS: runExamineSerial

runGenerate:
	./generator.o datafile 1000000

runExamineParallel:
	./examineParallel.o -1 -1 datafile -1 -1

runExamineSerial:
	./examineSerial.o -1 -1 datafile -1 -1

# CLEAN COMMANDS
# vvvvvvvvvvvvvv

clean: cleanE cleanS cleanDatafile cleanGenerate cleanClusterFiles

cleanG: cleanGenerate

cleanE: cleanExamineParallel

cleanS: cleanExamineSerial

cleanT: cleanClusterFiles

cleanDatafile:
	rm -rf datafile 

cleanExamineParallel:
	rm -rf examineParallel.o

cleanExamineSerial:
	rm -rf examineSerial.o

cleanGenerate:
	rm -rf generator.o

cleanClusterFiles:
	rm -rf testing.*
