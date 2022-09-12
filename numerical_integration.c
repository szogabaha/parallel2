#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <pthread.h>
#include <string.h>

#define MAX_ARG_LEN 32

/** Calculation process:
 * 1) Evaluate the value of the function in x1 to get b1. b1 = f(x1)
 * 2) Evaluate the value of the function in x2 to get b2. b2 = f(x2)
 * 3) h = x2 - x1
 */

/** Distribution of the work
 * 1) the main thread calculates how many trapezoids each thread need to calculate: n = N/T
 * 2) each thread has a local variable as a partial sum that will add to the entire sum (which is protected by a mutex)
 */

const double INTERVAL = 1.0;

static int threads_number;
static int trapezoids_number;

// this must be an atomic variable
static double area_sum = 0.0;

// function 4/(1+x^2)
static inline double test_func(double x)
{
    return 4 / (1 + (x * x));
}

// Area of trapezoid
static inline double trapezoid_area(double b1, double b2, double h)
{
    return ((b1 + b2) * h) / 2;
}

static inline double interval_step(int trapezoids_number)
{
    return (double)INTERVAL / trapezoids_number;
}

void usage(void)
{
    printf("\nUsage:\n./numerical_integration -t T -n N\n");
    printf("Where:\n");
    printf("\tT: number of the threads\n");
    printf("\tN: number of the trapezoids\n");
}

void parse_args(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Missing arguments!\n");
        usage();
        exit(0);
    }
    else if (strncmp("-t", argv[1], MAX_ARG_LEN))
    {
        printf("Unrecognized argument %s\n", argv[1]);
        usage();
        exit(0);
    }
    else if((threads_number = atoi(argv[2])) == 0) // the user insert zero or non-valid string
    {
        printf("Invalid threads number!\n");
        usage();
        exit(0);
    }
    else if(strncmp("-n", argv[3], MAX_ARG_LEN))
    {
        printf("Unrecognized argument %s\n", argv[3]);
        usage();
        exit(0);
    }
    else if((trapezoids_number = atoi(argv[4])) == 0) // the user insert zero or non-valid string
    {
        printf("Invalid trapezoids number!\n");
        usage();
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    parse_args(argc, argv);
    // lets make sequential first
    int max_trapezoids_number = 10000000;

    for (size_t trapezoid_number = 1; trapezoid_number <= max_trapezoids_number; trapezoid_number += 100000)
    {
        double sum = 0.0;
        printf("--- Area with %ld trapezius ---\n", trapezoid_number);
        double step = interval_step(trapezoid_number);
        double d = 0.0;
        double h = step;
        int count = 0;
        while (count < trapezoid_number)
        {
            double b1 = test_func(d);
            d += h;
            double b2 = test_func(step);
            step += h;
            sum += trapezoid_area(b1, b2, h);
            count++;
        }
        printf("%.36f\n", sum);
    }
    printf("math.h PI=3.141592653589793238462643383279502884\n");
}