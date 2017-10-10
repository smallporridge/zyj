#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#define WRITER_NUM 4		//writer number
#define READER_NUM 4		//reader number

int readercount = 0 ;		//number of in reading
int writercount=0;		//number of in writing
sem_t rsem,x,y,z,wsem;		

void writer(void *arg)
{
	while (1)
	{	
		sleep(1);
		sem_wait(&y);
		writercount++;
		if(writercount==1)//the first writer wait which in reading
			sem_wait(&rsem);
		sem_post(&y);
		sem_wait(&wsem);
		printf("writer %d is writing\n", (int)arg);
		printf("writer %d finish\n", (int)arg);
		sem_post(&wsem);
		sem_wait(&y);
		writercount--;
		if(writercount==0)
			sem_post(&rsem);
		sem_post(&y);
		sleep(1);		
	}
}


void reader(void *arg)
{
	while (1)
	{
		
		sem_wait(&z); //wait which in writing
		sem_wait(&rsem);//wait which will read
		sem_wait(&x);//wait which in reading
		readercount++;
		if(readercount==1)//the first reader wait which in writing
			sem_wait(&wsem);
		
		sem_post(&x);
		sem_post(&rsem);			
		sem_post(&z);
		printf("reader %d is reading\n", (int)arg);
		
		printf("reader %d finish\n", (int)arg);
		
		sem_wait(&x);
		readercount--;
		if(readercount==0)
			sem_post(&wsem);
		
		sem_post(&x);
		sleep(1);
	}
}

int main()
{
	pthread_t writers[WRITER_NUM];	
	pthread_t readers[READER_NUM];	
	sem_init(&x,0,1);		
	sem_init(&y,0,1);
	sem_init(&z,0,1);
	sem_init(&rsem,0,1);
	sem_init(&wsem,0,1);
	for (int i = 0; i < WRITER_NUM; i++)//create writer pthread
	{
		pthread_create(&writers[i], NULL, writer, (void*)i); 
	}
	for (int i = 0; i < READER_NUM; i++)//create reader pthread
	{
		pthread_create(&readers[i], NULL, reader, (void*)i);
	}
		
	for(int i=0;i<WRITER_NUM;i++)
	{
		pthread_join(writers[i],NULL);
	}
	for(int i=0;i<READER_NUM;i++)
	{
		pthread_join(readers[i],NULL);
	}
}
