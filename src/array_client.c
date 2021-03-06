/*
 * Structures et fonctions relatives à une collection de clients
 * Auteurs : Pierre Gaultier & Thomas Minier
 */
#include "array_client.h"

/*
 * Initialise une collection de clients
 */
void array_client_init(array_client_t *array_client, size_t size) {
  array_client->clients = malloc(sizeof(array_client_t) * size);
  memset(array_client->clients, 0, size);
  pthread_mutex_init(&array_client->lock, NULL);
  array_client->size = size;
  array_client->count = 0;
}

/*
 * Libère une collection de clients
 */
void array_client_free(array_client_t *array_client) {
  int i;
  pthread_mutex_lock(&array_client->lock);
  /* free all the clients */
  for (i = 0; i < array_client->count; i++) {
    pthread_cancel(array_client->clients[i]->client_thread);
    pthread_mutex_destroy(&array_client->clients[i]->lock);
    close(array_client->clients[i]->socket);
    free(array_client->clients[i]);
  }
  /* cleanup the rest of the struct */
  free(array_client->clients);
  pthread_mutex_unlock(&array_client->lock);
  pthread_mutex_destroy(&array_client->lock);
  free(array_client);
}

/*
 * Ajoute un client à une collection de clients
 */
int array_client_add(array_client_t *array_client, int client_socket) {
  client_t *new_client;

  pthread_mutex_lock(&array_client->lock);

  /* Si la tableau est plein, on l'agrandit */
  if (array_client->count == array_client->size) {
    array_client->size *= 2;
    array_client->clients =
        realloc(array_client->clients, array_client->size * sizeof(client_t));
  }

  new_client = malloc(sizeof(client_t));
  new_client->socket = client_socket;
  new_client->is_logged = 0;
  memset(new_client->username, 0, MAX_USERNAME_SIZE);
  pthread_mutex_init(&new_client->lock, NULL);
  array_client->clients[array_client->count] = new_client;
  array_client->count++;

  pthread_mutex_unlock(&array_client->lock);
  return array_client->count - 1;
}

/*
 * Set le nom d'un client
 */
int array_client_setName(array_client_t *array_client, int client_socket,
                         char *name) {
  int i;
  int client_ind;
  pthread_mutex_lock(&array_client->lock);

  /* recherche du client en question */
  for (i = 0; i < array_client->count; i++) {
    if (array_client->clients[i]->socket == client_socket) {
      pthread_mutex_lock(&array_client->clients[i]->lock);
      memcpy(array_client->clients[i]->username, name, strlen(name) + 1);
      pthread_mutex_unlock(&array_client->clients[i]->lock);
      client_ind = i;
    }
  }

  pthread_mutex_unlock(&array_client->lock);
  return client_ind;
}

/*
 * Supprime un client d'une collection
 */
int array_client_delete(array_client_t *array_client, int client_socket) {
  int i;
  int client_ind;
  client_t *client;
  pthread_mutex_lock(&array_client->lock);

  /* recherche du client en question */
  for (i = 0; i < array_client->count; i++) {
    if (array_client->clients[i]->socket == client_socket) {
      client = array_client->clients[i];
      client_ind = i;
    }
  }
  pthread_mutex_destroy(&client->lock);
  free(client);

  array_client_compact(array_client, client_ind, array_client->count);
  array_client->count--;
  pthread_mutex_unlock(&array_client->lock);
  return client_ind;
}

/*
 * Tasse une portion d'une collection de clients
 */
void array_client_compact(array_client_t *array_client, int start, int size) {
  int i;
  for (i = start; i < size - 1; i++) {
    array_client->clients[i] = array_client->clients[i + 1];
  }
}
