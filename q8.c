// Write an MPI program to calculate the sum of array using Scatter and Gather

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int n = 8; // number of elements in the array
    int data[n];
    int sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        // Initialize the data array
        for (int i = 0; i < n; i++)
        {
            data[i] = i + 1;
        }
    }

    // Calculate the number of elements to scatter to each process
    int count = n / size;
    int local_data[count];

    // Scatter the data array to all processes
    MPI_Scatter(data, count, MPI_INT, local_data, count, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the local sum
    int local_sum = 0;
    for (int i = 0; i < count; i++)
    {
        local_sum += local_data[i];
    }

    // Gather the local sums to the root process
    MPI_Gather(&local_sum, 1, MPI_INT, &sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Sum: %d\n", sum);
    }

    MPI_Finalize();
    return 0;
}
