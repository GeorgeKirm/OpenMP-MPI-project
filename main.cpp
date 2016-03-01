#include <omp.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    int tid;
    #pragma omp parallel shared(tid)
    {
        printf("Hello world from thread %d\n", tid);
    }
    return 0;
}