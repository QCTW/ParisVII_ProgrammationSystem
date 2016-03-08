#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>

#include "rendezvous.h"

volatile sig_atomic_t quitter = 0;

void
quit_handler(int signo)
{
    quitter = 1;
}

int
main()
{
    struct sigaction sa;
    struct rendezvous *r;
    int fd, rc;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = quit_handler;
    sa.sa_flags = 0;

    rc = sigaction(SIGINT, &sa, NULL);
    if(rc < 0) {
        perror("sigaction");
        exit(1);
    }

    fd = shm_open(NAME, O_RDWR | O_CREAT, 0666);
    if(fd < 0) {
        perror("shm_open");
        exit(1);
    }

    rc = ftruncate(fd, sizeof(struct rendezvous));
    if(rc < 0) {
        perror("ftruncate");
        shm_unlink(NAME);
        exit(1);
    }

    r = mmap(NULL, sizeof(struct rendezvous), PROT_READ | PROT_WRITE,
             MAP_SHARED, fd, 0);
    if(r == MAP_FAILED) {
        perror("mmap");
        shm_unlink(NAME);
        exit(1);
    }

    close(fd);

    sem_init(&r->vide, 1, 1);
    sem_init(&r->plein, 1, 0);

    while(!quitter) {
        rc = sem_wait(&r->plein);
        if(rc < 0) {
            if(errno == EINTR)
                continue;
            perror("sem_wait");
            exit(1);
        }
        printf("%d\n", r->value);
        do {
            rc = sem_post(&r->vide);
        } while(rc < 0 && errno == EINTR);
        if(rc < 0) {
            perror("sem_post");
            exit(1);
        }
    }

    shm_unlink(NAME);

    return 0;
}
