#include <stdio.h>
#include "pclient.h"

void array_compact(client_t ** array, int start, int size) {
	int i;
	for(i = start; i < size - 1; i++) {
		array[i] = array[i+1];
	}
}

void * pclient_add(void * socket_data) {
	int sock = (intptr_t) socket_data;
	if(storage.size < NB_CLIENTS) {
		pthread_mutex_lock(&storage.mutex_lock);
		// allocate the new client
		client_t * new_client = malloc(sizeof(client_t));
		new_client->id = sock;
		sem_init(&new_client->sem_lock, 0, 1);
		sem_wait(&new_client->sem_lock);
		// register the new client
		storage.clients[storage.size] = new_client;
		storage.size++;
		pthread_mutex_unlock(&storage.mutex_lock);
		printf("Client ajouté , socket %d!\n", sock);
	} else {
		printf("Erreur : nombre de clients max atteint\n");
	}
}

void * pclient_renvoi(void * sock_data) {
	int sock = (intptr_t) sock_data;
    char buffer[256];
    int longueur;
	int i;

    if ((longueur = read(sock, buffer, sizeof(buffer))) <= 0) {
	    return;
	}

    printf("message lu : %s \n", buffer);

    buffer[0] = 'R';
    buffer[1] = 'E';
    buffer[longueur] = '#';
    buffer[longueur+1] ='\0';

    printf("message apres traitement : %s \n", buffer);

    printf("renvoi du message traite.\n");

    /* mise en attente du prgramme pour simuler un delai de transmission */
    sleep(3);

    write(sock,buffer,strlen(buffer)+1);
		//write(sock, sock, strlen(sock) + 1);

    printf("message envoye. \n");

	// unlock the semaphore
	for(i = 0; i < storage.size; i++) {
		if(storage.clients[i]->id == sock) {
			sem_post(&storage.clients[i]->sem_lock);
		}
	}
}

void * pclient_leave(void * sock_data) {
	int sock = (intptr_t) sock_data;
	int i;
	client_t * client;

	// DEBUG
	printf("Array avant :\n");
	for(i = 0; i < storage.size; i++) {
		printf("elt %d\n", storage.clients[i]->id);
	}

	// search for the client
	for(i = 0; i < storage.size; i++) {
		if(storage.clients[i]->id == sock) {
			client = storage.clients[i];
		}
	}
	// waiting for the signal "The client has leave the room"
	sem_wait(&client->sem_lock);

	pthread_mutex_lock(&storage.mutex_lock);
	// cleaning up the semaphore & the client
	sem_post(&client->sem_lock);
	sem_destroy(&client->sem_lock);
	free(storage.clients[i]);
	// cleaning up the clients array
	array_compact(storage.clients, i, storage.size);
	storage.size--;
	pthread_mutex_unlock(&storage.mutex_lock);

	// DEBUG
	printf("client parti\n Array après : \n");
	for(i = 0; i < storage.size; i++) {
		printf("elt %d\n", storage.clients[i]->id);
	}

	// closing the socket
	printf("closing the socket %d\n", sock);
	close(sock);

}
