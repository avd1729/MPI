// Write a program to demonstrate features of some of the OPENMP library functions in C

#include <stdio.h>
#include <omp.h>

int main()
{
    double start, end;
    start = omp_get_wtime();
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Thread %d is executing the parallel region\n", tid);
    }
    end = omp_get_wtime();
    printf("Execution time of the parallel region: %f seconds\n", end - start);
    return 0;
}
