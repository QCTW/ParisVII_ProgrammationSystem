/*
 * client.c
 *
 *  Created on: 8 mars 2016
 *      Author: Quincy
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <unistd.h>     /* pause */
#include <fcntl.h>      /* open */
#include <sys/file.h>
#include <sys/socket.h> /* socket, bind*/
#include <sys/types.h>
#include <sys/un.h>     /* sockaddr_un */
#include <signal.h>
#include <string.h>     /* memset */

const char socketPath[14] = "/tmp/soquette";

int main (int argc, char* argv[])
{
	int socketFd = socket(PF_LOCAL, SOCK_STREAM, 0); //In Mac PF_LOCAL=PF_UNIX
	if(socketFd==-1)
		perror("Create socket failed");
	else
		printf("Socket %d created.\n", socketFd);

	struct sockaddr_un addr;
	memset((void*)&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path, "%s", socketPath);
	int connrc = connect(socketFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(connrc<0)
		perror("Unable to connect to socket");

	printf( "Chat %d started...\n", getpid() );

	char msg[256];
	//sprintf(msg, "Bonjour de client %d", getpid());
	while(1)
	{
		int nRead = read(STDIN_FILENO, msg, 256);
		msg[nRead-1] = 0;
		//scanf( "%s", msg );
		send(socketFd, msg, strlen(msg), 0);
		printf("Message '%s' sent to socket %d.\n", msg, socketFd);
	}
	close(connrc);
}
