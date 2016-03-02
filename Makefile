CC=g++

all: generate examine
examine: examineBuild examineRun
examineBuild:
	$(CC) -fopenmp main.cpp -o main.out
examineRun:
	./main.out -1 -1 datafile -1 -1
generate: generateBuild generateRun
generateBuild:
	$(CC) -std=c++11 generator.cpp -o generator.out
generateRun:
	./generator.out datafile 15000000
clean:
	rm -rf *o all
