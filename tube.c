/*
 * tube.cpp
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
#include <string.h>

void signal_callback_handler(int signum)
{
	printf("Stopped process:%d\n", getpid());
	exit(signum);
}

int main (int argc, char* argv[])
{
	char* msgPapa = "Salut, papa!";
	char* msgFiston = "Salut, fiston!";
	int fildesfather[2];
	int fildeschild[2];
	int pipefather = pipe(fildesfather);
	int pipechild = pipe(fildeschild);
	signal(SIGTSTP, signal_callback_handler);
	//close(fildeschild[1]); //If we close the fields before use it, the result becomes strange
	//close(fildesfather[0]);
	//close(fildeschild[0]);
	//close(fildesfather[1]);
	int pid = fork();
	if(pid==0)
	{
		printf("Child process created.");
		while(1)
		{
			write(fildeschild[1], msgPapa, strlen(msgPapa));
			printf("Child process yield: %s\n", msgPapa);
			char out[1024];
			int rd = read(fildesfather[0], out, 1024);
			out[rd] = '\0';
			printf("Child process heard: %s\n", out);
		}

	}
	else
	{
		printf("Father process.");
		while(1)
		{
			write(fildesfather[1], msgFiston, strlen(msgFiston));
			printf("Father process yield: %s\n", msgFiston);
			char out[1024];
			int rd = read(fildeschild[0], out, 1024);
			out[rd] = '\0';
			printf("Father process heard: %s\n", out);
		}

	}
}
