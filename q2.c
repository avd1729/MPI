// Write a OpenMP program in C to add numbers from 1 to 1000 and observe the result

#include <stdio.h>
#include <omp.h>

int main()
{
    int sum = 0;

// OpenMP parallel region with a reduction clause
#pragma omp parallel for reduction(+ : sum)
    for (int i = 1; i <= 1000; ++i)
    {
        sum += i;
    }

    printf("The sum of numbers from 1 to 1000 is: %d\n", sum);

    return 0;
}
