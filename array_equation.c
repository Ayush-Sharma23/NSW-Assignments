#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>


#define N 10

int main()
{
	int i, j;
	double *c, *x, *y;
	double a;
	double exe_time;
	struct timeval stop_time, start_time;
	
	c = (double *) malloc(N*sizeof(double));
	x = (double *) malloc(N*sizeof(double));
	y = (double *) malloc(N*sizeof(double));
	a = 0.01;
	
	for(i=0;i<N;i++)
	{
	    x[i] = i;
	    y[i] = i;
	    c[i] = 0;
	}
	printf("\n Initial Array :\n");
	for(i=0;i<N;i++)
	{
	    printf("%lf,%lf\t", x[i], y[i]);
	}
	
	gettimeofday(&start_time, NULL);
	
	for(i=0;i<N;i++)
	{
		c[i] = a*x[i] + y[i];
	}
	
	gettimeofday(&stop_time, NULL);	
	exe_time = (stop_time.tv_sec+(stop_time.tv_usec/1000000.0)) - (start_time.tv_sec+(start_time.tv_usec/1000000.0));
	
	printf("\n Final Array :\n");
	for(i=0;i<N;i++)
	{
	    printf(" %lf", c[i]);
	}
	printf("\n");
	free(c);
	free(x);
	free(y);
}

