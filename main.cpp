#include <omp.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    int tid;
    #pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();
        printf("Hello world from thread %d\n", tid);
    }
    return 0;
}
