#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>

#define N 10

int main(int argc, char **argv)
{
    int size, myid;
    int i;
    double *c, *x, *y;
    double *local_c, *local_x, *local_y;
    double a = 0.01;
    double exe_time;
    struct timeval stop_time, start_time;
    int local_n;
    int *sendcounts, *displs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Allocate arrays for counts and displacements
    sendcounts = (int *) malloc(size * sizeof(int));
    displs = (int *) malloc(size * sizeof(int));

    // Calculate local array sizes and displacements
    int base_n = N / size;
    int remainder = N % size;
    int offset = 0;
    for (i = 0; i < size; i++) {
        sendcounts[i] = (i < remainder) ? base_n + 1 : base_n;
        displs[i] = offset;
        offset += sendcounts[i];
    }
    local_n = sendcounts[myid];

    // Allocate global arrays only on root process
    if (myid == 0) {
        c = (double *) malloc(N * sizeof(double));
        x = (double *) malloc(N * sizeof(double));
        y = (double *) malloc(N * sizeof(double));

        // Initialize arrays
        for (i = 0; i < N; i++) {
            x[i] = i;
            y[i] = i;
            c[i] = 0;
        }

        printf("\nInitial Array:\n");
        for (i = 0; i < N; i++) {
            printf("%lf,%lf\t", x[i], y[i]);
        }
        printf("\n");
    }

    // Allocate local arrays
    local_c = (double *) malloc(local_n * sizeof(double));
    local_x = (double *) malloc(local_n * sizeof(double));
    local_y = (double *) malloc(local_n * sizeof(double));

    // Scatter the arrays using MPI_Scatterv
    MPI_Scatterv(x, sendcounts, displs, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(y, sendcounts, displs, MPI_DOUBLE, local_y, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Time the parallel computation
    gettimeofday(&start_time, NULL);

    // Perform local computation
    for (i = 0; i < local_n; i++) {
        local_c[i] = a * local_x[i] + local_y[i];
    }

    // Gather results using MPI_Gatherv
    MPI_Gatherv(local_c, local_n, MPI_DOUBLE, c, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    gettimeofday(&stop_time, NULL);
    exe_time = (stop_time.tv_sec + (stop_time.tv_usec / 1000000.0)) - 
               (start_time.tv_sec + (start_time.tv_usec / 1000000.0));

    // Print results from root process
    if (myid == 0) {
        printf("\nFinal Array:\n");
        for (i = 0; i < N; i++) {
            printf("%lf ", c[i]);
        }
        printf("\nExecution time: %lf seconds\n", exe_time);

        // Free global arrays
        free(c);
        free(x);
        free(y);
    }

    // Free local arrays and auxiliary arrays
    free(local_c);
    free(local_x);
    free(local_y);
    free(sendcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}