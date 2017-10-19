/*
 * Examen2015.c
 *
 *  Created on: 2016年5月24日
 *      Author: Quincy
 *
 *  If you found errors or have any question about the code,
 *  Please feel free to email quincy.tw(at)gmail.com. Thanks!
 */
#include <stdlib.h>
#include <fcntl.h>  /* open() */
#include <stdio.h>  /* printf() */
#include <unistd.h> /* fork() */
#include <sys/wait.h> /* wair() */

int main()
{
	  int foo = open("foo.txt", O_RDONLY);
	  int results = open("results.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	  int tube[2];
	  if(pipe(tube)<0)
		  perror("Creating pipe");
	  pid_t pere = fork();
	  if(pere==0)
	  {
	    close(results);
	    close(tube[0]);
	    close(STDIN_FILENO);
	    dup(foo);
	    close(STDOUT_FILENO);
	    dup(tube[1]);
	    if(execlp("grep","grep","howdy",NULL)<0)
	    	perror("Exec grep");
	    exit(0);
	  }
	  pid_t pere2 = fork();
	  if(!pere2)
	  {
	    close(foo);
	    close(tube[1]);
	    close(0);
	    dup(tube[0]);
	    close(1);
	    dup(results);
	    execlp("wc", "wc", "-l", NULL);
	  }
	  close(tube[0]);
	  close(tube[1]);
	  wait(NULL);
	  wait(NULL);
	  close(foo);
	  close(results);
	  return 0;
}

int main1 (int argc, char* argv[])
{
	int pid = fork();
	if(pid==0)
	{
		printf("Child process %d parent %d\n", pid, getppid());
	}
	else
	{
		printf("Father process %d parent %d\n", pid, getppid());
	}

	int pid2 = fork();
	if(pid2==0)
	{
		int nStatus = -1;
		printf("Child process %d-%d parent %d\n", pid, pid2, getppid());
		while(1)
		{
			printf("Child process waits %d-%d parent %d\n", pid, pid2, getppid());
			waitpid(getppid(), &nStatus, 0);
			sleep(6);
			printf("Child process %d-%d 's father died(%d). Current parent %d\n", pid, pid2, nStatus, getppid());
			break;
		}
	}
	else
	{
		printf("Father process %d-%d parent %d\n", pid, pid2, getppid());
		int count = 3;
		while(count)
		{
			sleep(1);
			count--;
		}
	}
	_exit(0);
}
