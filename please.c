/*
 * please.cpp
 *
 *  Created on: 2 févr. 2016
 *      Author: Quincy
 */

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <unistd.h>

int main (int argc, char* argv[])
{
	printf("%s \n", argv[0]);
	for(int i=0; i<5; i++)
	{
		int pid = fork();
		int nRet = -1;
		if(pid==0)
		{
			//nRet = execvp(argv[1], argv+1); //This code is useless because the value of nRet is just a copy in child process where father process can not access and not assigned
			execvp(argv[1], argv+1); //argv[1] means only the cell value of cell 1, while argv+1 means all the rest value from the pointer +1
		}

		wait(&nRet); //Note that here the value of nRet is filled by the return code of the child process (where execvp executes)
		printf("Terminaison avec résultat: %d \n", nRet);
		if (WIFEXITED(nRet))
			printf("Terminaison normal: %d\n", WEXITSTATUS(nRet));
		else if (WIFSIGNALED(nRet))
			printf("Commande tuée par le signal: %d\n", WTERMSIG(nRet));
		else
			printf("Bizarre return code!\n");
	}
	return 0;
}
