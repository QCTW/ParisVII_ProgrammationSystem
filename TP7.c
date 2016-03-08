/*
 * TP7.c
 *
 *  Created on: 8 mars 2016
 *      Author: Quincy
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <signal.h>

void signal_callback_handler(int signum)
{
	printf("La belle: Merci mon prince #%d !\n", signum);
	exit(signum);
}

int main (int argc, char* argv[])
{
	int rc = fork();
	if(rc==0) //Child process
	{
		//signal(SIGUSR1, signal_callback_handler);
		struct sigaction act;
		act.sa_handler = signal_callback_handler;
		act.sa_flags = 0;
		sigaction(SIGUSR1, &act, NULL); //In Mac there are 3 parameters
		printf("La belle: Humm...zzZz\n");
		pause();
	}
	else
	{
		printf("Le price: Ma belle #%d j'arrive pour vous!\n", rc);
		int count = 5;
		while(count>0)
		{
			printf("Le price: Ma belle #%d j'arrive dans %d secs!\n", rc, count);
			sleep(1);
			count--;
		}
		kill(rc, SIGUSR1);
	}
	return 0;
}
