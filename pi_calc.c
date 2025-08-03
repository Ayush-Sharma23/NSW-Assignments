#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>


#define N 9999999

int main(int argc, char **argv)
{
	int i, j;
    int size,myid;
    int splits;
	double area, pi;
	double dx, y, x;
	double total_area=0;
	
	double currArea=0;
	
    //Initialize MPI environment 
	MPI_Init(&argc,&argv);
	
	//Get total number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	//Get my unique ID among all processes
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	splits = N/size;
	int start = myid*splits+1;
	int end = start+splits;

	if (end>N){
		end=N;
	}

	dx = 1.0/N;
	x = 0.0;
	area = 0.0;


	for(i=start;i<end;i++)
	{
		x = i*dx;
		y = sqrt(1-x*x);
		area += y*dx;
	}
    
    if (myid==0){
            total_area=area;
            printf("\nmyid: %d \t Area calculated from process %d: %lf sq\n",myid,myid,area);
            for (int i=1; i<size;i++){
            	MPI_Recv(&currArea,1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            	printf("\nmyid: %d \t Area received from process %d: %lf sq\n", myid, i, currArea);
            	total_area += currArea;
            }
            
            pi = 4.0*total_area;
            printf("\n Value of pi is = %.16lf\n \n", pi);
    }
    else{
    	MPI_Send(&area,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }

    MPI_Finalize();
	
	
	
	return 0;
}

