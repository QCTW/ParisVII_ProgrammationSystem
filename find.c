/*
 * find.cpp
 *
 *  Created on: 2 févr. 2016
 *      Author: Quincy
 */
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void find(char *dir)
{
    DIR* dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr,"Unable to open directory: %s\n", dir);
        return;
    }

    while((entry = readdir(dp)) != NULL)
    {
    	char* dirName = entry->d_name;
        lstat( dirName, &statbuf);
        char* newDir = (char*) malloc(1024);
        sprintf(newDir, "%s/%s", dir, dirName);
        if(S_ISDIR(statbuf.st_mode))
        {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".", dirName) == 0 || strcmp("..", dirName) == 0)
                continue;

            printf("Directory: %s\n", dirName);
            /* Recurse at a new indent level */
            int pid = fork();
            if(pid==0)
            {
            	find(newDir);
            }
            else
            	wait(NULL);
        }
        else
        {
        	printf("File: %s\n", dirName);
        	if(statbuf.st_size >= 1000)
        	{
        		printf("Zip big file: %s\n", dirName);
        		int pid = fork();
				if(pid==0)
				{
					execlp("gzip", newDir, NULL);
				}
				else
					wait(NULL);
        	}
        }
    	free(newDir);
    }
    closedir(dp);
}

int main (int argc, char* argv[])
{
	find(argv[1]);
	return 0;
}
