/*
 * TP8.c
 *
 *  Created on: 15 mars 2016
 *      Author: Quincy
 *
 *  If you found errors or have any question about the code,
 *  Please feel free to email quincy.tw(at)gmail.com. Thanks!
 */
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* system, EXIT_FAILURE, random */
#include <signal.h>     /* sigaction, SIG_IGN */
#include <unistd.h>     /* exec */
#include <fcntl.h>      /* open */
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int array[255] = {0};

void signal_callback_handler(int signum)
{
	printf("Caught signal %d\n", signum);
	if(signum == SIGINT)
	{
		int nMax = 0;
		int nMaxPos = 0;
		int nMin = 0;
		int nMinPos = 0;
		for(int i=0; i < sizeof(array)/sizeof(int); i++)
		{
			if(nMin==0 && array[i]!=0)
				nMin = array[i];

			if(array[i] >=nMax)
			{
				nMax = array[i];
				nMaxPos= i;
			}

			if(array[i] <= nMin)
			{
				nMin = array[i];
				nMinPos = i;
			}
		}
		printf("Max(%d):%d Min(%d):%d\n", nMaxPos, nMax, nMinPos, nMin);
	}
	else if(signum == SIGQUIT)
	{
		for(int i=0; i < sizeof(array)/sizeof(int); i++)
		{
			printf("%d ", array[i]);
			if((i+1)%10==0)
				printf("\n");
		}
		printf("\n");
		exit(signum);
	}
}

int main (int argc, char* argv[])
{
	struct sigaction act;
	act.sa_handler = signal_callback_handler;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL); //In Mac there are 3 parameters
	sigaction(SIGQUIT, &act, NULL); //In Mac there are 3 parameters

	int rc = open("/dev/random", O_RDONLY, NULL);
	if(rc<0)
		perror("open(/dev/random, O_RDONLY)");

	unsigned char buff;
	while(1)
	{
		int rcread = read(rc, &buff, 1);
		if(rcread!=1)
			perror("read(rc, &buff, 1)");

		array[buff]++;
	}
}


int main_exe1 (int argc, char* argv[])
{
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigaction(SIGHUP, &act, NULL); //In Mac there are 3 parameters
	execvp(argv[1], argv+1);
	return 0;
}
