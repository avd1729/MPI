// Write a OpenMP program that counts the number of prime numbers from 1 to N.

#include <stdio.h>
#include <omp.h>

int is_prime(int num)
{
    if (num <= 1)
        return 0;

    for (int i = 2; i * i <= num; ++i)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int main()
{
    int N = 1000; // Change this value to the desired range

    int prime_count = 0;

// OpenMP parallel region with a reduction clause
#pragma omp parallel for reduction(+ : prime_count)
    for (int i = 1; i <= N; ++i)
    {
        if (is_prime(i))
        {
            prime_count++;
        }
    }

    printf("Number of prime numbers from 1 to %d: %d\n", N, prime_count);

    return 0;
}
