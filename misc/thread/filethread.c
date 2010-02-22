#include <iostream.h>
#include <fstream.h>
#include <pthread.h>
#include <sched.h>

const int BUFSIZE = 100;
const int MAXFILES = 3;
int linecount = 0;
void *parseFile(void *);
int main()
{
	pthread_t tid[MAXFILES];
	int i;
	
	pthread_create((&tid[0]),NULL,parseFile,"a.txt");
	pthread_create((&tid[1]),NULL,parseFile,"b.txt");
	pthread_create((&tid[2]),NULL,parseFile,"c.txt");
	
	for(i = 0; i < MAXFILES; i++)
	{
		pthread_join(tid[i],(void **) NULL);
	}
	cout<<"Num of lines = "<<linecount<<endl;
	return 0;
}

void *parseFile(void *filename)
{
	fstream file;
	char buffer[BUFSIZE];
	pthread_mutex_t mutex;
	
	file.open((char *)filename,ios::in);
	while(!file.eof())
	{
		file.getline(buffer,BUFSIZE);
		
		pthread_mutex_lock(&mutex);
		linecount++;
		pthread_mutex_unlock(&mutex);
	}
	file.close();
}
