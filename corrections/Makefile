CFLAGS = -pthread -Wall -O
LDLIBS = -lrt

.PHONY: all

all: serveur client

serveur: serveur.c

client: client.c

.PHONY: clean
clean:
	-rm -f client client.o serveur serveur.o *~ core
