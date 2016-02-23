/*
 * TP4.c
 *
 *  Created on: 16 févr. 2016
 *      Author: Quincy
 */

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/wait.h>



int fdId;

void delete()
{
	if(remove("/tmp/test_verrou")==0)
		printf("File /tmp/test_verrou(%d) deleted\n", fdId);
}

void unlock()
{
	flock(fdId, LOCK_UN);
}

void signal_callback_handler(int signum)
{
	printf("Caught signal:%d \n", signum);
	unlock();
	delete();
	exit(signum);
}

int _main3 (int argc, char* argv[])
{
	fdId = -1;
	fdId = open("/tmp/test_verrou", O_CREAT | O_RDWR | O_TRUNC, 0644 );
	flock(fdId, LOCK_EX);
	int pidFork = fork();
	printf("Fork process with pid:%d\n", pidFork);
	if( pidFork == 0 )
	{
		int count = 10;
		while(count>0)
		{
			sleep(1);
			printf("Child process slept 1: %d\n", pidFork);
		}
	}
	else
	{
		sleep(1);
		int retClose = close(fdId);
		printf("Parent(%d) close file desc: %d with return code: %d\n", pidFork, fdId, retClose);
	}

	unlock();
	delete();
	return 0;
}


int main (int argc, char* argv[])
{
	fdId = -1;
	fdId = open("/tmp/test_verrou", O_CREAT | O_RDWR | O_TRUNC, 0644 );
	flock(fdId, LOCK_EX);
	int fdDup = dup(fdId);
	printf("Open file /tmp/test_verrou with return code: %d. Duplicated id:%d\n", fdId, fdDup);

	if( fdId > 0 )
	{
		int retOpen = open("/tmp/test_verrou",  O_CREAT | O_RDWR | O_TRUNC, 0644 );
		printf("Open again file /tmp/test_verrou with return code: %d\n", retOpen);
		//int retClose = close(fdDup);
		//printf("Close file desc: %d with return code: %d\n", fdDup, retClose);
		sleep(10);
		int retClose = close(retOpen);
		printf("Close file desc: %d with return code: %d\n", retOpen, retClose);
	}

	unlock();
	delete();
	return 0;
}
