/*
 * si.cpp
 *
 *  Created on: 2 févr. 2016
 *      Author: Quincy
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <unistd.h>
#include <ctype.h>

int main (int argc, char* argv[])
{
	int status = -1;
	int pid = fork();
	if(pid==0)
	{
		execlp(argv[1], argv[1], NULL);
	}
	wait(&status);
	printf("Execute command: %s with return value: $d\n", argv[1], status);
	if(status==0)
		execlp(argv[2], argv[2], NULL);
	else
		execlp(argv[3], argv[3], NULL);
}
