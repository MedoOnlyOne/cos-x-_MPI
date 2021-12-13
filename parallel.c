#include <stdio.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

long double fact(long double x);

void main(int argc, char* argv[]){
    int rank, size;
    MPI_Status status;
    long double i = 0,
                local_i = 0,
                x_deg = 0,
                x_rad = 0,
                local_x_rad = 0,
                cos = 0,
                local_cos = 0;
    long int interval = 0,
             start = 0,
             end = 0;
    double time = 0,
           local_time = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        printf("\nEnter the value of i (upper limit): ");
        fflush(stdout);
        scanf("%Lf", &i);
        printf("Enter the value of x (angle in degrees): ");
        fflush(stdout);
        scanf("%Lf", &x_deg);
        
        x_rad = (x_deg * M_PI) / 180;

        for (int counter = 1; counter < size; counter++){
            MPI_Send(&i, 1, MPI_LONG_DOUBLE, counter, 0, MPI_COMM_WORLD);
            MPI_Send(&x_rad, 1, MPI_LONG_DOUBLE, counter, 1, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&local_i, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_x_rad, 1, MPI_LONG_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    }
    
    clock_t tic = clock();

    if (rank != 0){
        interval  = (long int) floor(local_i/size);
    } else {
        interval  = (long int) floor(i/size);
    }

    start = interval * rank;
    end = start + interval;
    
    for (long int k = start; k < end; k++){
        if ((rank == size - 1) && (k = end -1) && (k < local_i)){
            while(k < local_i){
                local_cos += ( (pow(-1, k) * pow(x_rad, 2*k)) / (fact(2*k)) );
                k++;
            }
            break;
        }
        local_cos += ( (pow(-1, k) * pow(x_rad, 2*k)) / (fact(2*k)) );
    }

    clock_t toc = clock();
    local_time = (double)(toc - tic) / CLOCKS_PER_SEC;

    if (rank != 0){
        MPI_Send(&local_cos, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&local_time, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    } else {
        cos = local_cos;
        time = local_time;
        for (int i = 1; i < size; i++){
            MPI_Recv(&local_cos, 1, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&local_time, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
            cos += local_cos;
            time += local_time;
        }
        printf("Processes number: %d\ncos(%Lf) = %Lf\nTime  = %lf seconds\n", size, x_deg, cos, time);
        fflush(stdout);
    }
    MPI_Finalize();
}

long double fact(long double x){
    long double fact_x = 1;
    for (long int i = x; i > 1; i--)
        fact_x *= i;
    return fact_x;
}
