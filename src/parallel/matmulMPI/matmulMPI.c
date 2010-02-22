/*
Author: W. Sharif
Date: 23/4/2006
Parallel Matrix Mutiplication Program
*/

#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define MASTER 0
int **alloc(int,int); 
int main(int argc,char *argv[])
{
	int SIZE;
	int numtasks, rank;
	int **a,**b;
	int *ta;
	int **result;
	int rem,i,j,k;
	int perProc;
	int *scounts,*displ;
	int **q,**r;
	int *tq,*tr,*tresult;
	struct timeval start,end,elapsed;
	
	/* Initialize MPI environment */
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, & numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(argc < 2)
	{
		if(rank == MASTER)
			printf("Usage: mpirun -np _ exe arraySize\n");
		exit(0);		
	}
	
	SIZE = atoi(argv[1]);
	MPI_Barrier(MPI_COMM_WORLD);
	a = alloc(SIZE,SIZE);
    b = alloc(SIZE,SIZE);
        
	/* if Master then Initialize array A & array B */
	if(rank == MASTER)
	{		
		if(SIZE < 1)
		{
			printf("Invalid square Matrix sizei\n");
			MPI_Finalize();
			exit(0);
		}
		if(SIZE <= 5)
		{
			printf("Enter values for Matrix A:\n");
			for(i = 0;i < SIZE;i++)
			{
				for(j = 0;j < SIZE;j++)
				{
					printf("Enter value for a[%d][%d]:",i,j);
					scanf("%d",&a[i][j]);
				}
			}
			printf("Enter values for Matrix B:\n");
			for(i = 0;i < SIZE;i++)
			{
					for(j = 0;j < SIZE;j++)
				{
					printf("Enter value for b[%d][%d]:",i,j);
					scanf("%d",&b[i][j]);
				}
			}
		}
		else
		{
			for(i = 0;i < SIZE;i++)
			{
				for(j = 0;j < SIZE;j++)
				{
					a[i][j] = rand() % 5;
					b[i][j] = rand() % 5;
				}
			}
		}
		printf("Num of processors used : %d\n",numtasks);	
	}

	/*Getting time at the start of computation */
    gettimeofday(&start,NULL); 
	
	/* Dividing Rows of array A evenly among all Processors */
	perProc = SIZE / numtasks;
	rem = SIZE % numtasks;
	scounts = (int *) malloc(numtasks*sizeof(int));
	displ = (int *) malloc(numtasks*sizeof(int));
	for(i = 0;i < numtasks;i++)
	{
		if(rem > 0)
			scounts[i] = perProc+1;
		else
			scounts[i] = perProc;
		if(i == 0)
			displ[i] = 0;
		else
			displ[i] = displ[i-1] + (scounts[i-1] * SIZE);
		rem--;
	}		

	/* Calcualting no. of rows to compute by this particular Processor */
	for(i = 0;i < numtasks;i++)
		scounts[i] *= SIZE;

	/* We have to pinpoint every pointer to array */
	ta = &a[0][0];
	q = alloc(scounts[rank] / SIZE,SIZE);	
	tq = &q[0][0];
	r = alloc(scounts[rank] / SIZE,SIZE);	
	tr = &r[0][0];
	result = alloc(SIZE,SIZE);	
	tresult = &result[0][0];

	/* Scatter array A among the Processors */
	MPI_Scatterv(ta,scounts,displ,MPI_INT,tq,scounts[rank],MPI_INT,0,MPI_COMM_WORLD);

	/* Broadcast  array B to all Processors */
	MPI_Bcast(*b,SIZE * SIZE,MPI_INT,0,MPI_COMM_WORLD);
	
	/*Compute individual Rows */
	for(i = 0;i < scounts[rank] / SIZE;i++)
	{
		for(j = 0;j < SIZE;j++)
		{
			r[i][j] = 0;
			for(k = 0;k < SIZE;k++)
			{
				r[i][j] += q[i][j] * b[k][j];
			}
		}	
	}
	
	/* Gather results from all Processors */
	MPI_Gatherv(tr,scounts[rank],MPI_INT,tresult,scounts,displ,MPI_INT,0,MPI_COMM_WORLD);
      	
	/*Getting time at the end of computation */
	gettimeofday(&end,NULL); 
        
	/* if Master then print array A,array B & array RESULT */
	if(rank == MASTER)
	{
		if(SIZE <= 5)
		{
			/*Printing Array A*/
			for(i = 0;i < SIZE;i++)
			{
				for(j = 0;j < SIZE;j++)
				printf("%d ",a[i][j]);
				printf("\n");
			}
			printf("\n");
			
			/*Printing Array B*/
			for(i=0;i < SIZE;i++)
			{
				for(j = 0;j < SIZE;j++)
				printf("%d ",b[i][j]);
				printf("\n");
			}
			printf("\n");
			
			/*Printing Array RESULT*/
			for(i = 0;i < SIZE;i++)
			{
				for(j = 0;j < SIZE;j++)
				printf("%d ",result[i][j]);
				printf("\n");
			}
		}
		if (start.tv_usec > end.tv_usec)
	    { 
   			end.tv_usec += 1000000; 
   			end.tv_sec--; 
		} 

		elapsed.tv_usec = end.tv_usec - start.tv_usec; 
		elapsed.tv_sec  = end.tv_sec  - start.tv_sec; 
	
		/* Final Result */
		printf("\nTotal time taken (%u.%06u) with problem size of %d ,using %d processors\n",elapsed.tv_sec,elapsed.tv_usec,SIZE,numtasks);
	}
	
	/* All done exit MPI */
	MPI_Finalize();
	return 0;
}

/* Function to allocate memory */
int **alloc(int r, int c) 
{
    int **arr,i;
    arr = (int **)malloc(r * sizeof(int *));
    arr[0] = (int *)malloc(r * c * sizeof(int));
    for(i = 1; i< r;i++)
	    arr[i] = arr[i-1] + c;
	
	return arr;
}
