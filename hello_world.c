#include"stdio.h"
#include"mpi.h"

#define N 100

int main(int argc, char **argv)
{
	int myid, size;
	
	//Initialize MPI environment 
	MPI_Init(&argc,&argv);
	
	//Get total number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	//Get my unique ID among all processes
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    int splits = N/size;
    int start = (myid)*splits;
    int end = (myid+1)*splits;
    
    
	
	printf("myid = %d : start = %d \t end = %d.\n",myid,start,end);
	
	//End MPI environment        
	MPI_Finalize();
}
