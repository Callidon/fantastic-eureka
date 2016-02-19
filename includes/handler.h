#ifndef HANDLER_H
#define HANDLER_H

#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "config.h"
#include "decoder.h"
#include "array_client.h"

typedef struct {
	int socket;
	array_client_t * array_client;
} client_datas_t;

typedef struct {
	int socket;
	WINDOW *window;
} render_datas_t;

/*
 * Méthode qui gère la réception et le traitement des messages reçus côté serveur
 */
void * server_handler(void * client_datas);

/*
 * Méthode qui gère la réception et le traitement des messages reçus côté client
 */
void * client_handler(void * render_datas);

#endif
