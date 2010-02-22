/*
Author: W. Sharif
Date: 23/4/2006
Sequential Matrix Mutiplication Program
*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

int **alloc(int,int); 
int main(int argc,char *argv[])
{
	int SIZE;
	int numtasks = 1;
	int **a,**b;
	int **result;
	int i,j,k;
	struct timeval start,end,elapsed;
	
	SIZE = atoi(argv[1]);
	a = alloc(SIZE,SIZE);
    b = alloc(SIZE,SIZE);
	result = alloc(SIZE,SIZE);	
	   
	if(SIZE < 1)
	{
		printf("Invalid square Matrix sizei\n");
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
				
				/*Make ArrayB as Identity Matrix*/
				if(i == j)
					b[i][j] = 1;
				else
					b[i][j] = 0;
				}
		}
	}	
	
	/*Getting time at the start of computation */
    gettimeofday(&start,NULL); 
	
	/*Compute individual Rows */
	for(i = 0;i < SIZE;i++)
	{
		for(j = 0;j < SIZE;j++)
		{
			result[i][j] = 0;
			for(k = 0;k < SIZE;k++)
			{
				result[i][j] += a[i][j] * b[k][j];
			}
		}	
	}
	
	/*Getting time at the end of computation */
	gettimeofday(&end,NULL); 
        
	/* if Master then print array A,array B & array RESULT */
	if(SIZE <= 50)
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
