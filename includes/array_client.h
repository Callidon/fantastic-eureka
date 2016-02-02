#ifndef ARRAY_CLIENT_H
#define ARRAY_CLIENT_H

#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int socket;
	pthread_t client_thread;
	pthread_mutex_t lock;
} client_t;

typedef struct {
	size_t size;
	size_t count;
	client_t ** clients;
	pthread_mutex_t lock;
} array_client_t;

void array_client_init(array_client_t * array_client, size_t size);

void array_client_free(array_client_t * array_client);

int array_client_add(array_client_t * array_client, int client_socket);

int array_client_delete(array_client_t * array_client, int client_socket);

void array_client_compact(array_client_t * array_client, int start, int size);

#endif
