#ifndef PCLIENT_H
#define PCLIENT_H
#define NB_CLIENTS 5

#include <stdint.h>
#include <pthread.h>

typedef struct {
	int clients [NB_CLIENTS];
	int size;
	pthread_t threads [NB_CLIENTS];
	pthread_mutex_t mutex_lock;
} storage_t;

static storage_t storage = {
	.size = 0,
	.mutex_lock = PTHREAD_MUTEX_INITIALIZER,
};

void * pclient_add(void * client_id);

void * pclient_leave(void * client_id);

#endif
