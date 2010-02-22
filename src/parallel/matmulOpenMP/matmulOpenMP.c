#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>

int **alloc(int,int);
int main(int argc,char *argv[])
{
	int size = 5;
	int **a,**b,**result;
	int i,j,k;
	struct timeval start,end,elapsed;
	int NPROCS = omp_get_num_procs();
	
	if(argv[1] != NULL)
		size = atoi(argv[1]);
	if(argv[2] != NULL)
		NPROCS = atoi(argv[2]);
	
	omp_set_num_threads(NPROCS);
	
	a = alloc(size,size);
	b = alloc(size,size);
	result = alloc(size,size);
	
	for(i = 0;i < size;i++)
	{
		//#pragma omp parallel for
		for(j = 0;j < size;j++)
		{
			a[i][j] = rand() % size;
			
			/*Make ArrayB as Identity Matrix*/
			if(i == j)
				b[i][j] = 1;
			else
				b[i][j] = 0;
		}
	}

	printf("This machine has got %d processors \n",omp_get_num_procs());
	printf("My thread id is %d\n",omp_get_thread_num());
	printf("Using %d threads in this parallel block\n",omp_get_num_threads());
	
	/*Getting time at the start of computation */
    gettimeofday(&start,NULL); 
    
    #pragma omp parallel for private(j,k)
    for(i = 0;i < size;i++)
	{
		for(j = 0;j < size;j++)
		{
			result[i][j] = 0;
			for(k = 0;k < size;k++)
			{
				result[i][j] += a[i][j] * b[k][j];
			}
		}	
	}
	
	/*Getting time at the end of computation */
	gettimeofday(&end,NULL); 
	
	if (start.tv_usec > end.tv_usec)
    { 
		end.tv_usec += 1000000; 
		end.tv_sec--; 
	} 

	elapsed.tv_usec = end.tv_usec - start.tv_usec; 
	elapsed.tv_sec  = end.tv_sec  - start.tv_sec; 

	/* Final Result */
	printf("\nTotal time taken (%u.%06u) with problem size of %d ,using %d processors\n",elapsed.tv_sec,elapsed.tv_usec,size,NPROCS);

	if(size <= 50)
	{
		/*Printing Array A*/
		for(i = 0;i < size;i++)
		{
			for(j = 0;j < size;j++)
			printf("%.2d ",a[i][j]);
			printf("\n");
		}
		printf("\n");
		
		/*Printing Array B*/
		for(i=0;i < size;i++)
		{
			for(j = 0;j < size;j++)
			printf("%.2d ",b[i][j]);
			printf("\n");
		}
		printf("\n");
		
		/*Printing Array RESULT*/
		for(i = 0;i < size;i++)
		{
			for(j = 0;j < size;j++)
			printf("%.2d ",result[i][j]);
			printf("\n");
		}
	}
		
		return 0;
}

int **alloc(int r, int c) 
{
    int **arr,i;
    arr = (int **)malloc(r * sizeof(int *));
    arr[0] = (int *)malloc(r * c * sizeof(int));
    for(i = 1; i< r;i++)
	    arr[i] = arr[i-1] + c;
	
	return arr;
}
