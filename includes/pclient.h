#ifndef PCLIENT_H
#define PCLIENT_H

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "array_client.h"

array_client_t * array_client;

int pclient_add(int sock_data);

void * pclient_renvoi(void * sock_data);

void * pclient_leave(void * sock_data);

#endif
