/*
 * lonely.cpp
 *
 *  Created on: 9 févr. 2016
 *      Author: Quincy
 */

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <signal.h>


int fdId;

void unlock_and_delete()
{
	flock(fdId, LOCK_UN);
	if(remove("/tmp/lonely.lock")==0)
		printf("File /tmp/lonely.lock(%d) deleted\n", fdId);
}

void signal_callback_handler(int signum)
{
	printf("Caught signal:%d \n", signum);
	unlock_and_delete();
	exit(signum);
}

int main (int argc, char* argv[])
{
	fdId = -1;
	fdId = open("/tmp/lonely.lock", O_CREAT | O_EXCL );
	printf("Open file /tmp/lonely.lock with return code: %d\n", fdId);
	if( fdId < 0 )
	{
		FILE *f = fopen("/tmp/lonely.lock","r");
		fseek (f, 0, SEEK_END);
		int length = ftell (f);
		fseek (f, 0, SEEK_SET);
		char* str = (char*)malloc (length);
		if (str)
			fread (str, 1, length, f);

		fclose (f);
		printf("Another lonely process (%s) already in execution. \n", str);
	}
	else
	{
		int pid = getpid();
		FILE *fptr = fopen("/tmp/lonely.lock","w");
		fprintf(fptr,"%d", pid);
		fclose(fptr);
		printf("Current pid (%d) written into lock file.\n", pid);
	}

	int lockId = -1;
	if( (lockId = flock(fdId, LOCK_EX)) < 0 )
	{
		printf("Unable to get file lock (%d) with fd ID: %d. Exit.\n", lockId, fdId);
		return 127;
	}

	signal(SIGTSTP, signal_callback_handler);
	while(1)
	{
		printf("Je suis seul et ai froid...\n");
		sleep(5);
	}

	unlock_and_delete();
	return EXIT_SUCCESS;
}
