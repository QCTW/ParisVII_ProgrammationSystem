/*
 * TP6.c
 *
 *  Created on: 1 mars 2016
 *      Author: Quincy
 */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE, EXIT_SUCCESS */
#include <sys/mman.h> /* mmap, msync */
#include <fcntl.h>    /* open */
#include <sys/stat.h> /* fstat */
#include <unistd.h>   /* ftruncate */
#include <string.h>   /* memcpy */


volatile int* sharedChar;
unsigned char* mapped;
struct stat stFile;

struct rendezvous
{
	int value;
	sem_t vide;
	sem_t plein;
};

int main (int argc, char* argv[])
{

}
