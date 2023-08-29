// Write an MPI program to find sum, odd and even of Fibonacci series.

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int n = 10; // number of terms in the Fibonacci series
    int fib[n];
    int sum = 0, odd = 0, even = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        // Generate the Fibonacci series
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i < n; i++)
        {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
    }

    // Broadcast the Fibonacci series to all processes
    MPI_Bcast(fib, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the local sum, odd and even numbers
    int local_sum = 0, local_odd = 0, local_even = 0;
    for (int i = rank; i < n; i += size)
    {
        local_sum += fib[i];
        if (fib[i] % 2 == 0)
        {
            local_even++;
        }
        else
        {
            local_odd++;
        }
    }

    // Reduce the local results to the root process
    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_odd, &odd, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_even, &even, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Fibonacci series: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", fib[i]);
        }
        printf("\nSum: %d\n", sum);
        printf("Number of odd numbers: %d\n", odd);
        printf("Number of even numbers: %d\n", even);
    }

    MPI_Finalize();
    return 0;
}
