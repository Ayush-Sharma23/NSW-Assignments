#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>


#define N 10

int main()
{
	int i, j;
	int *A;
	double exe_time;
	struct timeval stop_time, start_time;
	
	A = (int *) malloc(N*sizeof(int));
	
	for(i=0;i<N;i++)
	{
	    A[i] = i;
	}
	printf("\n Initial Array :\n");
	for(i=0;i<N;i++)
	{
	    printf(" %d", A[i]);
	}
	
	gettimeofday(&start_time, NULL);
	
	for(i=0;i<N;i++)
	{
		A[i] = A[i]*A[i];
	}
	
	gettimeofday(&stop_time, NULL);	
	exe_time = (stop_time.tv_sec+(stop_time.tv_usec/1000000.0)) - (start_time.tv_sec+(start_time.tv_usec/1000000.0));
	
	printf("\n Final Array :\n");
	for(i=0;i<N;i++)
	{
	    printf(" %d", A[i]);
	}
	printf("\n");
	free(A);
}

