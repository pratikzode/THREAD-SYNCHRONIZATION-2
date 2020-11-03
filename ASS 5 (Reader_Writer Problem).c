#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>

int readCount;
sem_t s, mSem;	

void* readerMethod(void* data)
{
	FILE* fp;
	char ch;	
	do
	{
		sem_wait(&s);
		readCount++;
		if(readCount==1)
			sem_wait(&mSem);
		sem_post(&s);
		
		fp=fopen("abc.txt", "r");
		ch=fgetc(fp);	
		while(!feof(fp))
		{
			printf("%c",ch);
			ch=fgetc(fp);
		}
		printf("\nReader Completed Execution!!\n");
		fclose(fp);
		
		sem_wait(&s);
		readCount--;
		if(readCount==0)
			sem_post(&mSem);
		sem_post(&s);
	}while(readCount!=0);
}

void* writerMethod(void* data)
{
	FILE *fp;
	char string[20];
	sem_wait(&mSem);
	
	printf("\nEnter the Text: ");
	scanf("%s", string);
		
	fp=fopen("abc.txt", "a");
	fprintf(fp, "%s", string);
	fclose(fp);		
	printf("\nWriter Has Written!!\n");	
	sem_post(&mSem);
}
		
int main()
{
	int r, i, n;
	FILE *fp;
	pthread_t writer, reader[10];
	sem_init(&s, 0, 1);
	sem_init(&mSem, 0, 1);
	
	printf("\nEnter The Number of Readers: ");
	scanf("%d", &n);

	fp=fopen("abc.txt","a");
	fclose(fp);
	
	readCount=0;	
	
	for(i=0;i<n;i++)
		pthread_create(&reader[i], NULL,readerMethod , NULL); 
	pthread_create(&writer, NULL, writerMethod ,NULL); 
	
	for(i=0; i<n;i++)
		pthread_join(reader[i], NULL);
	pthread_join(writer, NULL);
	return 0;
}
