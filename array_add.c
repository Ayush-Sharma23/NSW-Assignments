#include"stdio.h"
#include"mpi.h"
#include<stdlib.h>

#define ARRSIZE 20

int main(int argc, char **argv)
{
	int myid, size;
	int i;
	int a[ARRSIZE],b[ARRSIZE],c[ARRSIZE];
	int a_chunk[ARRSIZE/4], b_chunk[ARRSIZE/4],c_chunk[ARRSIZE/4];	
	
	//Initialize MPI environment 
	MPI_Init(&argc,&argv);
	
	//Get total number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	//Get my unique identification among all processes
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	//Exit the code if the number of processes is not equal to 4
	if(size!=4)
	{
		printf("\n Please use EXACTLY 4 processes!\n");
		MPI_Finalize();
		exit(0);
	}
	
	//If root
	if(myid==0)
	{
		//Initialize data to some value
		for(i=0;i<ARRSIZE;i++)
		{
			a[i] = i;
			b[i] = i;
			c[i] = 0;			
		}
		
		//print the data
		printf("\nInitial data: ");
		for(i=0;i<ARRSIZE;i++)
		{
			printf("\n%d \t %d", a[i], b[i]);			
		}
	}
	//Distribute / Scatter the data from root = 0
	MPI_Scatter(&a, ARRSIZE/4, MPI_INT, &a_chunk, ARRSIZE/4, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&b, ARRSIZE/4, MPI_INT, &b_chunk, ARRSIZE/4, MPI_INT, 0, MPI_COMM_WORLD);
	
	//Every process works on ARRSIZE/4 of data
	for(i=0;i<ARRSIZE/4;i++)
	{
		c_chunk[i] = a_chunk[i] + b_chunk[i];				
	}
	
	//Collect / Gather the data at root = 0 
	MPI_Gather(&c_chunk, ARRSIZE/4, MPI_INT, &c, ARRSIZE/4, MPI_INT, 0, MPI_COMM_WORLD);
	
	//If root
	if(myid==0)
	{
		//print the data
		printf("\nFinal data: ");
		for(i=0;i<ARRSIZE;i++)
		{
			printf("\t%d", c[i]);			
		}
		printf("\n\nProgram exit!\n");
	}
	
	//End MPI environment        
	MPI_Finalize();
}
