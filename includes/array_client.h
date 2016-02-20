/*
 * Structures et fonctions relatives à une collection de clients
 * Auteurs : Pierre Gaultier & Thomas Minier
 */
 #include <stdlib.h>
 #include <pthread.h>
 #include <string.h>
 #include "config.h"

#ifndef ARRAY_CLIENT_H
#define ARRAY_CLIENT_H


// Structure représentant un client
typedef struct {
	int socket;
	char username [MAX_USERNAME_SIZE];
	pthread_t client_thread;
	pthread_mutex_t lock;
} client_t;

// Structure représentant une collection de clients
typedef struct {
	size_t size;
	size_t count;
	client_t ** clients;
	pthread_mutex_t lock;
} array_client_t;

/*
 * Initialise une collection de clients
 */
void array_client_init(array_client_t * array_client, size_t size);

/*
 * Libère une collection de clients
 */
void array_client_free(array_client_t * array_client);

/*
 * Ajoute un client à une collection de clients
 */
int array_client_add(array_client_t * array_client, int client_socket);

/*
 * Set le nom d'un client
 */
int array_client_setName(array_client_t * array_client, int client_socket, char * name);

/*
 * Supprime un client d'une collection
 */
int array_client_delete(array_client_t * array_client, int client_socket);

/*
 * Tasse une portion d'une collection de clients
 */
void array_client_compact(array_client_t * array_client, int start, int size);

#endif
