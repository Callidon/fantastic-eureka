#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define NB_CLIENTS 5

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

static void * pth_add_client(void * client_id) {
	int id = (intptr_t) client_id;
	if(storage.size < NB_CLIENTS) {
		pthread_mutex_lock(&storage.mutex_lock);
		storage.clients[storage.size] = id;
		storage.size++;
		pthread_mutex_unlock(&storage.mutex_lock);
		printf("Client ajouté !\n");
	} else {
		printf("Erreur : nombre de clients max atteint\n");
	}
}
