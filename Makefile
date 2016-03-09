CC=gcc

all: generate examine
examine: examineBuild examineRun
examineBuild:
	$(CC) -fopenmp main.c -o main.out
examineRun:
	./main.out -1 -1 datafile -1 -1
generate: generateBuild generateRun
generateBuild:
	$(CC) -Wall generator.c -o generator.out
generateRun:
	./generator.out datafile 1500
clean:
	rm -rf *.out datafile all
