/*
 * serveur.c
 *
 *  Created on: 8 mars 2016
 *      Author: Quincy
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/socket.h> /* socket, bind*/
#include <sys/types.h>
#include <sys/un.h>     /* sockaddr_un */
#include <signal.h>
#include <string.h>     /* memset */

int connection_handler(int connectionId)
{
    int nbytes;
    char buffer[256];

    nbytes = read(connectionId, buffer, 256);
    buffer[nbytes] = 0;

    printf("Message from client: %s\n", buffer);
    nbytes = snprintf(buffer, 256, "hello from the server");
    write(connectionId, buffer, nbytes);

    close(connectionId);
    return 0;
}

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
	addr.sun_len = sizeof(struct sockaddr_un);
	unlink(socketPath);
	snprintf(addr.sun_path, sizeof(addr.sun_path), socketPath);
	int brc = bind(socketFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(brc<0)
		perror("Unable to bind socket");

	int lrc = listen(socketFd, 0);
	if(lrc<0)
		perror("Unable to listen tp socket");
	else
		printf("Server is now listening on socket %d...\n", socketFd);

	socklen_t addrLength;
	int connFd = accept(socketFd, (struct sockaddr *) &addr, &addrLength);
	if(connFd<0)
		perror("Accept socket connection failed");

	/* now inside newly created connection handling process */
	connection_handler(connFd);

	/* still inside server process */
	close(connFd);
	unlink(socketPath);
	return 0;
}
