// Write  a program to print "Hello world" for multiple processes using parallel programming concept in OpenMP.

#include <stdio.h>
#include <omp.h>

int main()
{
    // Set the number of threads to use
    omp_set_num_threads(4); // You can adjust this number according to your system

// OpenMP parallel region
#pragma omp parallel
    {
        // Get the thread ID
        int thread_id = omp_get_thread_num();

        // Print "Hello, world!" along with the thread ID
        printf("Hello, world! This is thread %d\n", thread_id);
    }

    return 0;
}
