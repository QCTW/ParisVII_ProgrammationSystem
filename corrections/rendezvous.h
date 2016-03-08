#define NAME "rendezvous"

struct rendezvous {
    int value;
    sem_t vide;
    sem_t plein;
};

