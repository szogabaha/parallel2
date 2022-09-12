#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

/**Calculation process:
 * 1) Evaluate the value of the function in x1 to get b1. b1 = f(x1)
 * 2) Evaluate the value of the function in x2 to get b2. b2 = f(x2)
 * 3) h = x2 - x1
 * 4)
 */

const double INTERVAL = 1.0;

// function 4/(1+x^2)
static inline double test_func(double x)
{
    return 4 / (1 + (x * x));
}

// Area of trapezius
static inline double trapezius_area(double b1, double b2, double h)
{
    return ((b1 + b2) * h) / 2;
}

static inline double interval_step(int trapezius_number)
{
    return (double)INTERVAL / trapezius_number;
}

int main(int argc, char *argv[])
{
    // lets make sequential first
    int max_trapezius_number = 10000000;

    for (size_t trapezius_numbers = 1; trapezius_numbers <= max_trapezius_number; trapezius_numbers += 100000)
    {
        double sum = 0.0;
        printf("--- Area with %ld trapezius ---\n", trapezius_numbers);
        double step = interval_step(trapezius_numbers);
        double d = 0.0;
        double h = step;
        int count = 0;
        while (count < trapezius_numbers)
        {
            double b1 = test_func(d);
            d += h;
            double b2 = test_func(step);
            step += h;
            sum += trapezius_area(b1, b2, h);
            count++;
        }
        printf("%.36f\n", sum);
    }
    printf("math.h PI=3.141592653589793238462643383279502884\n");
}