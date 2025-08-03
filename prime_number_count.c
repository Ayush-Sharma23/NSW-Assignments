#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>
#include<time.h>
#include<sys/time.h>

#define N 10240
/*
                N  PRIME_NUMBER

                1           0
               10           4
              100          25
            1,000         168
           10,000       1,229
          100,000       9,592
        1,000,000      78,498
       10,000,000     664,579
      100,000,000   5,761,455
    1,000,000,000  50,847,534

*/
int main(int argc, char **argv)
{
    int size, myid;
	int i, j;
    int splits;
    int recieved_count =0;
	int count=0, flag;
	double exe_time;
    int total_count=0;
	struct timeval stop_time, start_time;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	
	// gettimeofday(&start_time, NULL);
	
    if(myid == 0) // Process 0 only acts as aggregator
    {
        total_count = 0; // Start with 0, prime 2 will be counted by process 1
        
        // Receive counts from all other processes
        for (int source = 1; source < size; ++source) {
            MPI_Recv(&recieved_count, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("\nmyid: %d \t Data received from process %d: %d primes", myid, source, recieved_count);
            total_count += recieved_count;
        }
        
        printf("\nTotal count of prime numbers = %d\n", total_count);
        printf("\n\nProgram exit!\n"); 
    }
    else{
        // Worker processes start with count=0
        count = 0;
        
        // Divide work among processes 1 to (size-1)
        int worker_processes = size - 1;
        splits = N / worker_processes;
        int start = (splits * (myid - 1)) + 3;
        int end = start + splits;
        
        // Only process 1 accounts for prime number 2 to avoid double counting
        if (myid == 1) {
            count = 1; // Add 2 as a prime number
        }
        
        if (end>N){
            end = N;    
        }
        
        
        for(i=start;i<end;i++)
	       {
	 	         flag = 0;
		          for(j=2;j<i;j++)	
	           {
		               if((i%j) == 0)
		              {
			                  flag = 1;
			                   break;
		                   }
	                }
             if(flag == 0)
             {
                	count++;
             }
	       }
        MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	
	   }
	// gettimeofday(&stop_time, NULL);	
	// exe_time = (stop_time.tv_sec+(stop_time.tv_usec/1000000.0)) - (start_time.tv_sec+(start_time.tv_usec/1000000.0));
	// 
	// printf("\n Number of prime numbers = %d \n Execution time is = %lf seconds\n", count, exe_time);
	
	MPI_Finalize();
	return 0;
}

