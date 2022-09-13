#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define MAX_ARG_LEN 32

#define F(x) \
    4.0 / (1 + (x * x))

#define AREA(b1, b2, h) \
    ((b1 + b2) * h) / 2

const double INTERVAL = 1.0;

static int threads_number;
static int trapezoids_number;

pthread_mutex_t sum_mutex;
static double area_sum = 0.0;

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
    else if ((threads_number = atoi(argv[2])) == 0) // the user insert zero or non-valid string
    {
        printf("Invalid threads number!\n");
        usage();
        exit(0);
    }
    else if (strncmp("-n", argv[3], MAX_ARG_LEN))
    {
        printf("Unrecognized argument %s\n", argv[3]);
        usage();
        exit(0);
    }
    else if ((trapezoids_number = atoi(argv[4])) == 0) // the user insert zero or non-valid string
    {
        printf("Invalid trapezoids number!\n");
        usage();
        exit(0);
    }
}

void *hello_thread(void *parameters)
{
    int id = (intptr_t)parameters;
    printf("Hello from pthread %d\n", id);
    return NULL;
}

typedef struct thread_data
{
    int trapezoid_number;
    double h; // height of trapezoids
    double start_from;
} data_t;

void *calculate_integral(void *parameters)
{
    data_t *data = (data_t *)(parameters);
    printf("number:%d\nstart_from:%lf\nstep:%lf\n\n", data->trapezoid_number, data->start_from, data->h);
    double partial_sum = 0.0;
    int count = 0;
    double x0 = data->start_from;
    double x1 = x0 + data->h;
    while (count < data->trapezoid_number)
    {
        double b1 = F(x0);
        double b2 = F(x1);
        partial_sum += AREA(b1, b2, data->h);
        // update for the next cycle
        x0 += data->h;
        x1 += data->h;
        ++count;
    }
    // finish to calculate, lets update the global variable
    pthread_mutex_lock(&sum_mutex);
    area_sum += partial_sum;
    pthread_mutex_unlock(&sum_mutex);
    free(data);
    return NULL;
}

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    // Calculate the height of trapezoids
    double h = (double)INTERVAL / trapezoids_number;

    if (pthread_mutex_init(&sum_mutex, NULL) == -1)
    {
        printf("Failed to create mutex!\n");
        exit(0);
    }

    // Let's allocate the memory for the threads
    pthread_t *pthreads = (pthread_t *)malloc(sizeof(pthread_t) * threads_number);
    if (pthreads == NULL)
    {
        printf("Error allocating memory\n");
        exit(-1);
    }
    int trapezoids_per_thread = trapezoids_number / threads_number;
    for (int i = 0; i < threads_number; i++)
    {
        data_t *data = (data_t *)calloc(1, sizeof(data_t));
        data->trapezoid_number = trapezoids_per_thread;
        data->h = h;
        data->start_from = trapezoids_per_thread * i * h;
        if (pthread_create(&pthreads[i], NULL, calculate_integral, (void *)data))
        {
            printf("Failed to create pthread\n");
            goto cleanup;
        }
    }
    for (int i = 0; i < threads_number; i++)
    {
        pthread_join(pthreads[i], NULL);
    }

    printf("%28s:\t%.18f\n", "Integral approximated value", area_sum);
    printf("%28s:\t%s\n", "math.h PI", "3.14159265358979323846");

cleanup:
    free(pthreads);
}