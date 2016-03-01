#include <omp.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    int tid;
    #pragma omp parallel
    {
        printf("Hello world from thread %d\n", tid);
    }
    return 0;
}