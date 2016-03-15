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
#include <stdlib.h>     /* system, EXIT_FAILURE */
#include <signal.h>     /* sigaction, SIG_IGN */
#include <unistd.h>     /* exec */

int main (int argc, char* argv[])
{
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigaction(SIGHUP, &act, NULL); //In Mac there are 3 parameters
	execvp(argv[1], argv+1);
}
