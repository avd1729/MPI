// Write an OpenMp/Mpi program in c that takes data from process 0 and sends it to all other processes in a ring format

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank, size;
    int data;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        data = 100; // Data to be sent
        MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        printf("Process %d sent data %d to process %d\n", rank, data, rank + 1);
    }
    else
    {
        MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        printf("Process %d received data %d from process %d\n", rank, data, rank - 1);
        if (rank < size - 1)
        {
            MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            printf("Process %d sent data %d to process %d\n", rank, data, rank + 1);
        }
    }

    MPI_Finalize();
    return 0;
}
