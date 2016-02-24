/*
 * TP5.c
 *
 *  Created on: 23 févr. 2016
 *      Author: Quincy
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE, EXIT_SUCCESS */
#include <sys/mman.h> /* mmap */
#include <fcntl.h>    /* open */
#include <sys/stat.h> /* fstat */
#include <unistd.h>   /* ftruncate */
#include <string.h>   /* memcpy */


unsigned char* contentSrc;
unsigned char* contentDest;
int fdSrc;
int fdDest;
struct stat stSrc;
struct stat stDest;

int main (int argc, char* argv[])
{
	fdSrc = open(argv[1], O_RDONLY, 0666);
	fdDest = open(argv[2], O_CREAT | O_RDWR, 0666);
	if(fdSrc>0 && fdDest>0)
	{
		int src = fstat(fdSrc, &stSrc);
		int dest = fstat(fdDest, &stDest);

		if(stSrc.st_size != stDest.st_size) //Note: if we did not do this, contentDest will be MAP_FAILED and we are not able to use the memory with 0 size
			ftruncate(fdDest, stSrc.st_size);

		contentSrc = mmap(NULL, stSrc.st_size, PROT_READ, MAP_PRIVATE, fdSrc, 0);
		contentDest = mmap(NULL, stDest.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdDest, 0);

		printf("Original src file contents:\n%s\n", contentSrc);
		printf("Original src/dest file size: %d/%d\n", (int)stSrc.st_size, (int)stDest.st_size);
		printf("Original dest file contents:\n%s\n", contentDest);
		if(contentSrc != MAP_FAILED && contentDest != MAP_FAILED)
		{
			printf("Coping Src to Dest...\n");
			memcpy(contentDest, contentSrc, stSrc.st_size);
			printf("Copied dest file contents:\n%s\n", contentDest);
		}

		munmap(contentSrc, stSrc.st_size);
		munmap(contentDest, stDest.st_size);
		close(fdSrc);
		close(fdDest);
	}
	return EXIT_SUCCESS;
}

int main1 (int argc, char* argv[])
{
	fdSrc = open("./TP5.c", O_RDONLY);
	if(fdSrc>0)
	{
		int rc = fstat(fdSrc, &stSrc);
		contentSrc = mmap(NULL, stSrc.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdSrc, 0);
		if(contentSrc != MAP_FAILED)
		{
			printf("File contents:\n%s\n", contentSrc);
		}
	}
	return EXIT_SUCCESS;
}
