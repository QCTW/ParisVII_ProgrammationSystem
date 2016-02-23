/*
 * TP2.cpp
 *
 *  Created on: 2 févr. 2016
 *      Author: Quincy
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <unistd.h>

int main ()
{
	printf("TP2 Execution\n");
	int nRet = 0;
	int pid = fork();
	if(pid==0)
	{
		nRet = execlp("/bin/ls", "ls", "-ltr", NULL);
		printf("Child process do something\n"); //Code here will never execute because when execlp executed, the control is out of scope of current code
	}
	else
	{
		wait(&nRet); //Wait child process to terminate
		printf("Father process do nothing\n");
	}
	return 0;
}
