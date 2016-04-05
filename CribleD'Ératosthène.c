/*
 * CribleD'Ératosthène.c
 *
 *  Created on: 22 mars 2016
 *      Author: Quincy
 *
 *  If you found errors or have any question about the code,
 *  Please feel free to email quincy.tw(at)gmail.com. Thanks!
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 100000

pthread_t pthread[SIZE];
volatile int integers[SIZE];

pthread_mutex_t verrou;
pthread_cond_t condCrossedFirst;

void* barrer(void* args)
{
	int entier = *((int*) args);
	int index = entier;
	int fst = 1;
	while(index<SIZE)
	{
		index += entier;
		//printf("Thread %d is crossing cell %d.\n", entier, index);
		if(index >= SIZE) break;
		pthread_mutex_lock(&verrou);
		integers[index] = 0;
		if(fst)
		{
			//printf("Release condition\n");
			pthread_cond_broadcast(&condCrossedFirst);
			fst = 0;
		}
		pthread_mutex_unlock(&verrou);
	}
	if(fst){
		pthread_mutex_lock(&verrou);
		pthread_cond_broadcast(&condCrossedFirst);
		pthread_mutex_unlock(&verrou);
	}
	return NULL;
}

int main (int argc, char* argv[])
{
	int count = 0;
	for(int i=0; i<SIZE; i++)
	{
		integers[i] = i;
	}

	pthread_mutex_init(&verrou, NULL);
	pthread_cond_init(&condCrossedFirst, NULL);
	for(int i=2; i<SIZE; i++)
	{
		//printf("Read %d = %d\n", i , integers[i]);
		if(integers[i]>0)
		{
			count++;
			pthread_mutex_lock(&verrou);
			int rc = pthread_create(&pthread[i], NULL, barrer, &i);
			if(rc)
				printf("Error\n");
			printf("Thread %d created to cross its multiple.\n", i);
			pthread_cond_wait(&condCrossedFirst, &verrou);
			pthread_mutex_unlock(&verrou);
		}
	}

	for(int i=2; i<SIZE; i++)
	{
		if(integers[i]>0)
		{
			pthread_join(pthread[i], NULL);
		}
	}

	printf("==== All Prime Numbers within %d ====.\n", SIZE);
	int nCountPrimeNum = 0;
	for(int i=1; i<SIZE; i++)
	{
		if(integers[i]!=0)
		{
			nCountPrimeNum++;
			printf("%d ", integers[i]);
		}

		if(nCountPrimeNum==9)
		{
			nCountPrimeNum = 0;
			printf("\n");
		}
	}
	printf("\n");
	printf("%d of threads created\n", count);

}
