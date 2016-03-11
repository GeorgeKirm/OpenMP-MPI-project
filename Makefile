CC = gcc
CFLAGS = -Wall -g
LIBS = -L/path/to/libs -lm


all: generate examine examine2

buildG: generate

buildE: examine

buildS: examine2

run: generateRun examineRun

examine: main.c main.h
	$(CC) $(CFLAGS) -fopenmp main.c main.h -o main.o $(LIBS)

examine2: main2.c main2.h
	$(CC) $(CFLAGS) main2.c main2.h -o main2.o $(LIBS)

generate: generator.c generator.h
	$(CC) $(CFLAGS) generator.c generator.h -o generator.o

runE:
	./main.o -1 -1 datafile -1 -1

runS:
	./main2.o -1 -1 datafile -1 -1

runG:
	./generator.o datafile 100

clean:
	rm -rf *.o datafile

cleanE:
	rm -rf main.o

cleanS:
	rm -rf main2.o
