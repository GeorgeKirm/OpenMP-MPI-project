CC = gcc
MC = mpicc
CD = nvcc
CFLAGS = -Wall -g
LIBS = -L/path/to/libs -lm

# fast command
easy: buildGenerate buildExamineSerial2 runGenerate  runExamineSerial2

# BUILD COMMANDS
# vvvvvvvvvvvvvv

all: buildGenerate buildExamineSerial2 buildExamineSerial

buildG: buildGenerate

buildS: buildExamineSerial

buildC: buildExamineSerial2


buildExamineSerial: examineSerial.c examineSerial.h
	$(CC) $(CFLAGS) examineSerial.c examineSerial.h -o examineSerial.o $(LIBS)

buildExamineSerial2: examineSerial2.cu
	$(CD) examineSerial2.cu -o examineSerial2.o $(LIBS)

buildGenerate: generator.c generator.h
	$(CC) $(CFLAGS) generator.c generator.h -o generator.o

# RUN COMMANDS
# vvvvvvvvvvvv

run: runGenerate runExamineSerial2 runExamineSerial

runG: runGenerate

runS: runExamineSerial

runC: runExamineSerial2

runGenerate:
	./generator.o datafile 15000000

runExamineSerial:
	./examineSerial.o -1 -1 datafile -1 -1

runExamineSerial2:
	./examineSerial2.o -1 -1 datafile -1 -1

# CLEAN COMMANDS
# vvvvvvvvvvvvvv

clean: cleanC cleanS cleanDatafile cleanGenerate

cleanG: cleanGenerate

cleanS: cleanExamineSerial

cleanC: cleanExamineSerial2

cleanD: cleanDatafile

cleanDatafile:
	rm -rf datafile 

cleanExamineSerial:
	rm -rf examineSerial.o

cleanExamineSerial2:
	rm -rf examineSerial2.o

cleanGenerate:
	rm -rf generator.o
