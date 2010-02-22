#include <iostream.h>
#include <fstream.h>
#include <pthread.h>
#include <sched.h>
#define MAXTHREADS 2

int arr[20];
int count = 0;
int pcount = 0;
void *resample(void *);
void *overlap(void *);
int *getNextValue();
pthread_mutex_t mutex;
int main()
{
	pthread_t tid[MAXTHREADS];
	int i;
	
	pthread_create((&tid[0]),NULL,resample,NULL);
	pthread_create((&tid[1]),NULL,overlap,NULL);
	
	for(i = 0; i < MAXTHREADS; i++)
	{
		pthread_join(tid[i],(void **) NULL);
	}
	
	cout<<"Final output"<<endl;
	for(i = 0; i < 20; i++)
		cout<<arr[i]<<endl;
	
	return 0;
}

void *resample(void *a)
{
	int i;
	int *p = arr;
	for(i = 0; i < 20 ;i++) {
		sleep(1);
		pthread_mutex_lock(&mutex);
		p[i] = -1;
		pthread_mutex_unlock(&mutex);
		cout<<"Resample!!"<<endl;
	}
	cout<<"Resample done !!"<<endl;
}

void *overlap(void *a)
{
	sleep(15);
	int i;
	int *p;
	while(pcount < 20) {
		p = getNextValue();
		if(p == NULL)
			sleep(1);
		else {	
			p[pcount] = pcount;
			pcount++;
			cout<<"Overlap!!"<<endl;
		}
	}
	cout<<"Overlap done !!"<<endl;
}

int* getNextValue()
{
	if(arr[pcount] == -1)
		return arr;
	else 
		return NULL;
} 
