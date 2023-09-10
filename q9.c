//  Write an Mpi program to provide fair reception of message from all sending process using Waitsome

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUM_MESSAGES 4

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2)
    {
        fprintf(stderr, "This program requires at least 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0)
    {
        int recv_buffer[NUM_MESSAGES];
        MPI_Request requests[NUM_MESSAGES];
        MPI_Status statuses[NUM_MESSAGES];

        // Initialize receive requests
        for (int i = 0; i < NUM_MESSAGES; i++)
        {
            MPI_Irecv(&recv_buffer[i], 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i]);
        }

        int completed_count = 0;
        int completed_indices[NUM_MESSAGES];

        while (completed_count < NUM_MESSAGES)
        {
            MPI_Waitsome(NUM_MESSAGES, requests, &completed_count, completed_indices, statuses);

            // Process completed receives
            for (int i = 0; i < completed_count; i++)
            {
                int source = statuses[i].MPI_SOURCE;
                int tag = statuses[i].MPI_TAG;
                printf("Process 0 received message from process %d with tag %d: %d\n", source, tag, recv_buffer[completed_indices[i]]);
            }
        }
    }
    else
    {
        // Send messages from all processes other than rank 0
        for (int i = 1; i <= NUM_MESSAGES; i++)
        {
            MPI_Send(&rank, 1, MPI_INT, 0, i, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
