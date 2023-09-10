// Write a mpi program to implement Jacobi iteration for approximately the solution to a linear system of equations

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4 // Size of the linear system (change as needed)

void jacobi(double *A, double *b, double *x, double *x_new, int local_n, int rank, int size)
{
    double *local_x = (double *)malloc(local_n * sizeof(double));

    // Perform Jacobi iterations
    int max_iterations = 1000;
    double tolerance = 1e-6;
    int iter;
    for (iter = 0; iter < max_iterations; iter++)
    {
        // Compute local updates
        for (int i = 0; i < local_n; i++)
        {
            local_x[i] = b[i];
            for (int j = 0; j < N; j++)
            {
                if (j != rank * local_n + i)
                {
                    local_x[i] -= A[i * N + j] * x[j];
                }
            }
            local_x[i] /= A[i * N + rank * local_n + i];
        }

        // Gather updated local solutions to the root process
        MPI_Allgather(local_x, local_n, MPI_DOUBLE, x_new, local_n, MPI_DOUBLE, MPI_COMM_WORLD);

        // Compute the residual and check for convergence
        double residual = 0.0;
        for (int i = 0; i < N; i++)
        {
            double diff = x_new[i] - x[i];
            residual += diff * diff;
        }

        MPI_Bcast(&residual, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (residual < tolerance * tolerance)
        {
            break;
        }

        // Update the solution
        for (int i = 0; i < N; i++)
        {
            x[i] = x_new[i];
        }
    }

    free(local_x);
}

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = N / size;
    double *A = (double *)malloc(N * N * sizeof(double));
    double *b = (double *)malloc(N * sizeof(double));
    double *x = (double *)malloc(N * sizeof(double));
    double *x_new = (double *)malloc(N * sizeof(double));

    // Initialize A, b, and x (modify as needed)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i * N + j] = (i == j) ? 2.0 : 1.0;
        }
        b[i] = N + 1;
        x[i] = 0.0;
    }

    // Perform Jacobi iteration
    jacobi(A, b, x, x_new, local_n, rank, size);

    if (rank == 0)
    {
        printf("Solution:\n");
        for (int i = 0; i < N; i++)
        {
            printf("x[%d] = %lf\n", i, x[i]);
        }
    }

    free(A);
    free(b);
    free(x);
    free(x_new);

    MPI_Finalize();
    return 0;
}
