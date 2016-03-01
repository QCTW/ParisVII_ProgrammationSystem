/*
 * TP6.c
 *
 *  Created on: 1 mars 2016
 *      Author: Quincy
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE, EXIT_SUCCESS */
#include <sys/mman.h> /* mmap, msync, shm_open */
#include <fcntl.h>    /* open */
#include <sys/stat.h> /* fstat */
#include <unistd.h>   /* ftruncate */
#include <semaphore.h>   /* sem_t, sem_open */
#include <sys/types.h>   /* chmod, fchmod */
#include <sys/stat.h>    /* umask */

struct Rendezvous
{
	int value;
	sem_t vide;
	sem_t plein;
};

int main (int argc, char* argv[])
{
	const char* semaphoreName = "SemaphoreSHM";
	struct Rendezvous* rdv; //Pointer points to the memory created by mmap to be used for Rendezvous
	int memId = shm_open(semaphoreName, O_CREAT|O_RDWR, 0644);
	if( memId<0 )
	{
		printf("%d\n",memId);
	    perror("[2]shm_open() error");
	    exit(-1);
	}
	printf("[2]shm_open() created shared memory with id %d\n", memId);

	//Problem on MAC with 2nd execution of ftruncate
	//Check http://stackoverflow.com/questions/25502229/ftruncate-not-working-on-posix-shared-memory-in-mac-os-x
	struct stat mapstat;
	if (-1 != fstat(memId, &mapstat) && mapstat.st_size == 0)
	{
		if ( ftruncate(memId, sizeof(rdv)) < 0 )
		{
			perror("[2]ftruncate() error");
			exit(-1);
		}
	}
	printf("[2]ftruncate() shared memory to %lu\n", sizeof(rdv));

	rdv = mmap(NULL, sizeof(rdv), PROT_READ|PROT_WRITE, MAP_SHARED, memId, 0);
	if(rdv==MAP_FAILED)
	{
		perror("[2]Map failed");
		exit(-1);
	}

	printf("[2]mmap() created successful\n");
	rdv->value = 0;
	sem_init(&rdv->vide, 1, 1); //This does not work on MAC
	printf("[2]sem_init(&rdv->vide, 1, 1) executed\n");
	sem_init(&rdv->plein, 1, 0);
	printf("[2]sem_init(&rdv->plein, 1, 0) executed\n");
	//rdv->vide = *sem_open("Rdv_Vide", O_CREAT|O_EXCL, S_IWGRP|S_IWOTH, 1);
	//rdv->plein = *sem_open("Rdv_Plein", O_CREAT|O_EXCL, S_IWGRP|S_IWOTH, 0);
	while(1)
	{
		sem_wait(&rdv->plein);
		printf("[2]Rendezvous.value after sem_wait()=%d\n", rdv->value);
		rdv->value = rand()%100;
		printf("[2]Changed value=%d\n", rdv->value);
		sem_post(&rdv->vide);
		printf("[2]Rendezvous.value after sem_post()=%d\n", rdv->value);
		sleep(1);
	}
}
