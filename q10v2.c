#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MAX_ITER 1000
#define TOLERANCE 1e-6

// Function to perform Jacobi iteration
void jacobi(double *A, double *b, double *x, int n, int max_iter)
{
    int i, j, iter;
    double *new_x = (double *)malloc(n * sizeof(double));
    double sum;

    for (iter = 0; iter < max_iter; iter++)
    {
        for (i = 0; i < n; i++)
        {
            sum = 0.0;
            for (j = 0; j < n; j++)
            {
                if (i != j)
                {
                    sum += A[i * n + j] * x[j];
                }
            }
            new_x[i] = (b[i] - sum) / A[i * n + i];
        }

        // Check for convergence
        double error = 0.0;
        for (i = 0; i < n; i++)
        {
            error += fabs(new_x[i] - x[i]);
        }

        if (error < TOLERANCE)
        {
            break;
        }

        // Update x with the new values
        for (i = 0; i < n; i++)
        {
            x[i] = new_x[i];
        }
    }

    free(new_x);
}

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 2;              // Size of the linear system
    int local_n = n / size; // Number of rows per process

    // Allocate memory for A, b, and x
    double *A = (double *)malloc(n * n * sizeof(double));
    double *b = (double *)malloc(n * sizeof(double));
    double *x = (double *)malloc(n * sizeof(double));

    // Initialize A, b, and x based on the equations
    if (rank == 0)
    {
        A[0] = 1.0;
        A[1] = 1.0; // Coefficients for equation 1 (x + y = 10)
        A[2] = 1.0;
        A[3] = -1.0; // Coefficients for equation 2 (x - y = 2)

        b[0] = 10.0; // Right-hand side for equation 1
        b[1] = 2.0;  // Right-hand side for equation 2

        // Initial guess for x (you can set any initial values)
        x[0] = 0.0;
        x[1] = 0.0;
    }

    // Broadcast the initial values of A, b, and x to all processes
    MPI_Bcast(A, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform Jacobi iteration
    jacobi(A, b, x, n, MAX_ITER);

    // Print the final solution on rank 0
    if (rank == 0)
    {
        printf("Solution:\n");
        for (int i = 0; i < n; i++)
        {
            printf("x[%d] = %f\n", i, x[i]);
        }
    }

    // Clean up
    free(A);
    free(b);
    free(x);

    MPI_Finalize();
    return 0;
}
