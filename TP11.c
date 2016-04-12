/*
 * TP11.c
 *
 *  Created on: 12 avr. 2016
 *      Author: Quincy
 *
 *  If you found errors or have any question about the code,
 *  Please feel free to email quincy.tw(at)gmail.com. Thanks!
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILUR, random */
#include <pthread.h>

struct target
{
	int threadId;
	int startPos;
	int nbToCalculate;
	int* array;
};

pthread_mutex_t verrou;

void* somme(void* args)
{
	struct target t = *(struct target*) args;
	int* sum = malloc(sizeof(int));
	*sum = 0;
	for(int i = t.startPos; i<t.startPos+t.nbToCalculate; i++)
	{
		//pthread_mutex_lock(&verrou);
		*sum = *sum+t.array[i];
		printf("Thread_%d: Somme(%d)+=%d; startPos=%d; nbToCalculate=%d \n", t.threadId, i, t.array[i], t.startPos, t.nbToCalculate);
		//pthread_mutex_unlock(&verrou);
	}
	printf("Thread_%d: Sub-sum=%d\n", t.threadId, *sum);
	//return sum;
	pthread_exit(sum);
}

int main (int argc, char* argv[])
{
	srandom(time(NULL));
	int l = 10;
	int n = 3;
	int tab[l];
	for(int i=0; i<l; i++)
	{
		tab[i] = random() %10;
		printf("Random(%d)=%d\n", i, tab[i]);
	}

	pthread_mutex_init(&verrou, NULL);
	int nbPerThread = l/n;
	pthread_t pthread[n];
	for(int i=0; i<n; i++)
	{
		struct target* t = malloc(sizeof(struct target)); //malloc create new object in heap. Without malloc, we will create the same object in the stack
		t->threadId = i;
		t->startPos = i*nbPerThread;
		t->nbToCalculate = nbPerThread;
		if(i==n-1)
			t->nbToCalculate += l%n;
		t->array = tab;
		int rc = pthread_create(&pthread[i], NULL, somme, t);
		if(rc)
			perror("pthread_create error\n");
		else
			printf("pthread_create %d\n", i);
	}

	int final = 0;
	for(int i=0; i<n; i++)
	{
		void* res;
		pthread_join(pthread[i], &res);
		final += *(int*) res;
	}
	printf("Final sum=%d\n", final);
}
