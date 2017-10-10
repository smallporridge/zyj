#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#define WRITER_NUM 4		//writer number
#define READER_NUM 4		//reader number

int readercount = 0 ;		//number of in reading
int writercount=0;		//number of in writing
sem_t quemutex,fmutex,rcount;		

void writer(void *arg)
{
	while (1)
	{	
		sleep(1);
		sem_wait(&quemutex);
		sem_wait(&fmutex);
		printf("writer %d is writing\n", (int)arg);
		printf("writer %d finish\n", (int)arg);
		sem_post(&fmutex);
		sem_post(&quemutex);
		sleep(1);		
	}
}


void reader(void *arg)
{
	while (1)
	{
		
		sem_wait(&quemutex); 
		sem_wait(&rcount);
		readercount++;
		if(readercount==1)
			sem_wait(&fmutex);
		sem_post(&rcount);
		sem_post(&quemutex);			
		printf("reader %d is reading\n", (int)arg);
		
		printf("reader %d finish\n", (int)arg);
		
		sem_wait(&rcount);
		readercount--;
		if(readercount==0)
			sem_post(&fmutex);
		sem_post(&rcount);
		sleep(1);
	}
}

int main()
{
	pthread_t writers[WRITER_NUM];	
	pthread_t readers[READER_NUM];	
	sem_init(&quemutex,0,1);		
	sem_init(&rcount,0,1);
	sem_init(&fmutex,0,1);
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
