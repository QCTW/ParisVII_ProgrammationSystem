#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#include "rendezvous.h"

int
main()
{
    struct rendezvous *r;
    int fd;

    srandom(time(NULL));

    fd = shm_open(NAME, O_RDWR, 0666);
    if(fd < 0) {
        perror("shm_open");
        exit(1);
    }

    r = mmap(NULL, sizeof(struct rendezvous), PROT_READ | PROT_WRITE,
             MAP_SHARED, fd, 0);
    if(r == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    close(fd);

    sem_wait(&r->vide);
    r->value = random() % 42;
    sem_post(&r->plein);

    return 0;
}
