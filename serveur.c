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
#include <sys/select.h> /* select */
#include <sys/types.h>
#include <sys/un.h>     /* sockaddr_un */
#include <signal.h>
#include <string.h>     /* memset */

const char socketPath[14] = "/tmp/soquette";
int maxConnection = 4;
int maxSocketFd = -1;
int socketServerFd = -1;
int allSocketFd[4] = {-1, -1, -1, -1};
fd_set masterFdSet;

int addConnection(int socketId)
{
	for(int index=1; index<maxConnection; index++)
	{
		printf("Checking allSocketFd[%d]= %d\n", index, allSocketFd[index]);
		if(allSocketFd[index]==-1)
		{
			allSocketFd[index] = socketId;
			return index;
		}
	}
	return -1;
}

int main (int argc, char* argv[])
{
	socketServerFd = socket(PF_LOCAL, SOCK_STREAM, 0); //In Mac PF_LOCAL=PF_UNIX
	if(socketServerFd==-1)
		perror("Create socket failed");
	else
		printf("Socket %d created.\n", socketServerFd);

	unlink(socketPath);
	struct sockaddr_un addr;
	memset((void*)&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path, socketPath);
	int brc = bind(socketServerFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(brc<0)
		perror("Unable to bind socket");

	int lrc = listen(socketServerFd, 0);
	if(lrc<0)
		perror("Unable to listen to socket");
	else
		printf("Server is now listening on socket %d...\n", socketServerFd);

	FD_ZERO(&masterFdSet);
	maxSocketFd = socketServerFd;
	allSocketFd[0] = socketServerFd;
	FD_SET(socketServerFd, &masterFdSet);
	socklen_t addrLength;
	int connCount = 0;
	int temp = 5;
	while(temp)
	{
		temp--;
		fd_set readSet;
		FD_COPY(&masterFdSet, &readSet);
		int selectrc = select(maxSocketFd+1, &readSet, NULL, NULL, NULL);
		if(selectrc==-1)
			perror("Error on select");

		for(int index=0; index<maxConnection; index++)
		{
			int oneSocketFd = allSocketFd[index];
			if(oneSocketFd!=-1)
			{
				printf("Server checking FD(%d): %d\n", index, oneSocketFd);
				if(FD_ISSET(oneSocketFd, &readSet))
				{
					if(oneSocketFd==socketServerFd)
					{
						int socketClient = accept(socketServerFd, (struct sockaddr *) &addr, &addrLength);
						if(socketClient<0)
							perror("Accept socket connection failed");
						else
						{
							FD_SET(socketClient, &masterFdSet);
							int nAddConnAt = addConnection(socketClient);
							printf("Client %d connected. Stored in array[%d]/%d\n", socketClient, nAddConnAt, connCount);
							if(socketClient>maxSocketFd)
								maxSocketFd = socketClient;

							connCount++;
						}
					}
					else
					{
						char buffer[257];
						int nRecievedBytes = recv(oneSocketFd, &buffer, 256, 0);
						printf("Message size: %d\n", nRecievedBytes);
						if(nRecievedBytes!=0)
						{
							buffer[nRecievedBytes] = 0; //Add end of string in the recieved message
							printf("Message from client: %s\n", buffer);
						}
						else
						{
							allSocketFd[index] = -1;
							printf("Client %d closed connection: %d\n", oneSocketFd, index);
						}
					}
				}
			}
		}

	}
	unlink(socketPath);
	return 0;
}
