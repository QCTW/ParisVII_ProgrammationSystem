/*
 * TP9.c
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
#include <sys/time.h>

volatile int nShared = 0;
pthread_mutex_t verrou;
pthread_cond_t condEmpty;

void* decr(void* ptr)
{
	char* msg = (char*) ptr;
	int count = 10000;
	while(count>0)
	{
		pthread_mutex_lock(&verrou);
		while(nShared<=0)
		{
			pthread_cond_wait(&condEmpty, &verrou);
			printf("%s waits nShared become non-zero (%d)\n", msg, nShared);
		}

		nShared--;
		pthread_mutex_unlock(&verrou);
		count--;
		printf("%s decreased 1 to %d\n", msg, nShared);
	}
	return NULL;
}

void* incr(void* ptr)
{
	char* msg = (char*) ptr;
	int count = 10000;
	while(count>0)
	{
		pthread_mutex_lock(&verrou);
		nShared++;
		pthread_cond_broadcast(&condEmpty);
		pthread_mutex_unlock(&verrou);
		count--;
		printf("%s increased 1 to %d\n", msg, nShared);
	}
	return NULL;
}

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    return diff;
}

int main (int argc, char* argv[])
{
	struct timeval before , after;
	gettimeofday(&before , NULL);
	pthread_t t1, t2;
	char* pName1 = "Thread 1";
	char* pName2 = "Thread 2";
	pthread_cond_init(&condEmpty, NULL);
	pthread_mutex_init(&verrou, NULL);
	//int rc1 = pthread_create(&t1, NULL, print_message, (void*)pName1 );
	//int rc2 = pthread_create(&t2, NULL, print_message, (void*)pName2 );
	int rc1 = pthread_create(&t1, NULL, incr, (void*)pName1 );
	int rc2 = pthread_create(&t2, NULL, decr, (void*)pName2 );
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	gettimeofday(&after , NULL);
	printf("Final result=%d. Time=%f\n", nShared, time_diff(before, after));
	return 0;
}
