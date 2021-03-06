#include <stdio.h>
#include <time.h>
#include <math.h>

long double fact(long double x);

void main(void){
    long double i, x_deg, x_rad, cos = 0;
    printf("Enter the value of i (upper limit): ");
    scanf("%Lf", &i);
    printf("Enter the value of x (angle in degrees): ");
    scanf("%Lf", &x_deg);
    
    x_rad = (x_deg * M_PI) / 180;

    clock_t tic = clock();
    
    for (long int k = 0; k <= i; k++)
        cos += ( (pow(-1, k) * pow(x_rad, 2*k)) / (fact(2*k)) );

    clock_t toc = clock();
    double time_spent = (double)(toc - tic) / CLOCKS_PER_SEC;
    
    printf("cos(%Lf) = %Lf\nTime  = %lf seconds\n", x_deg, cos, time_spent);
}

long double fact(long double x){
    long double fact_x = 1;
    for (long int i = x; i > 1; i--)
        fact_x *= i;
    return fact_x;
}