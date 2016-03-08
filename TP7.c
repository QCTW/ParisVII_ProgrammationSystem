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
		signal(SIGUSR1, signal_callback_handler);
		while(1)
		{
			printf("La belle: Humm...\n");
			sleep(3);
		}
	}
	else
	{
		printf("Le price: Ma belle #%d j'arrive!\n", rc);
		sleep(10);
		kill(rc, SIGUSR1);
	}
	return 0;
}
