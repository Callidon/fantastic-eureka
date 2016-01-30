#ifndef PCLIENT_H
#define PCLIENT_H
#define NB_CLIENTS 5

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

typedef struct {
	int id;
	pthread_t client_thread;
	pthread_mutex_t lock;
	sem_t sem_lock;
} client_t;

typedef struct {
	client_t * clients [NB_CLIENTS];
	int size;
	pthread_t threads [NB_CLIENTS];
	pthread_mutex_t mutex_lock;
} storage_t;

static storage_t storage = {
	.size = 0,
	.mutex_lock = PTHREAD_MUTEX_INITIALIZER,
};

void * pclient_add(void * sock_data);

void * pclient_renvoi(void * sock_data);

void * pclient_leave(void * sock_data);

#endif
