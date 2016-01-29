#include <stdio.h>
#include "pclient.h"

void array_compact(int * array, int start, int size) {
	int i;
	for(i = start; i < size - 1; i++) {
		array[i] = array[i+1];
	}
}

void * pclient_add(void * client_id) {
	int id = (intptr_t) client_id;
	if(storage.size < NB_CLIENTS) {
		pthread_mutex_lock(&storage.mutex_lock);
		storage.clients[storage.size] = id;
		storage.size++;
		pthread_mutex_unlock(&storage.mutex_lock);
		printf("Client ajouté , socket %d!\n", id);
	} else {
		printf("Erreur : nombre de clients max atteint\n");
	}
}
// | 0 | 1 | 2 | 3 | 4 |
void * pclient_leave(void * client_id) {
	int id = (intptr_t) client_id;
	int i;
	pthread_mutex_lock(&storage.mutex_lock);
	printf("Array avant :\n");
	for(i = 0; i < storage.size; i++) {
		printf("elt %d\n", storage.clients[i]);
	}
	for(i = 0; i < storage.size; i++) {
		if(storage.clients[i] == id) {
			array_compact(storage.clients, i, storage.size);
			storage.size--;
		}
	}
	printf("client parti\n Array : \n");
	for(i = 0; i < storage.size; i++) {
		printf("elt %d\n", storage.clients[i]);
	}
	pthread_mutex_unlock(&storage.mutex_lock);
}
