/*
 * synchro.c
 *
 *  Created on: 23 févr. 2016
 *      Author: Quincy
 */

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE, EXIT_SUCCESS */
#include <sys/mman.h> /* mmap, msync */
#include <fcntl.h>    /* open */
#include <sys/stat.h> /* fstat */
#include <unistd.h>   /* ftruncate */
#include <string.h>   /* memcpy */


volatile int* sharedChar;
unsigned char* mapped;
struct stat stFile;

int main (int argc, char* argv[])
{
	int zero = 0;
	int fdSynchro = open("synchro.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if(fdSynchro>0)
	{
		printf("Open file synchro.txt: %d\n", fdSynchro);
		if ( write(fdSynchro, &zero, 1) != 1)
		{
			close(fdSynchro);
			perror("Error writing file synchro.txt");
			exit(EXIT_FAILURE);
		}
		printf("Write data in synchro.txt: %d\n", zero);
		fstat(fdSynchro, &stFile);
		mapped = mmap(NULL, stFile.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdSynchro, 0); //mapped[0] is 1 byte (octet)
		if(mapped != MAP_FAILED)
		{
			int pidFork = fork();
			if(pidFork==0) //Child process
			{
				sleep(5);
				int pid = getpid();
				printf("Child PID=%d\n", pid);
				*sharedChar = pid;
				msync(NULL, stFile.st_size, MS_SYNC);
			}
			else
			{
				int pid = getpid();
				while(*sharedChar==0)
				{
					sleep(1);
					printf("Parent PID=%d slept\n", pid);
				}
				printf("Parent get child's PID=%d\n", *sharedChar);
			}
			//Remove memory map
			munmap(mapped, stFile.st_size);
			close(fdSynchro);
		}
	}
	else
		printf("Unable to open file synchro.txt: %d\n", fdSynchro);
}
