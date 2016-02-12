#ifndef HANDLER_H
#define HANDLER_H

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

void * server_handler(void * client_datas);

#endif
