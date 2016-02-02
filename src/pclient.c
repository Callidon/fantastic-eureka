#include <stdio.h>
#include "pclient.h"

int pclient_add(int socket_data) {
	return array_client_add(array_client, socket_data);
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
}

void * pclient_leave(void * sock_data) {
	int sock = (intptr_t) sock_data;
	int i;

	// DEBUG
	printf("Array avant (count %d) :\n", array_client->count);
	for(i = 0; i < array_client->count; i++) {
		printf("elt %d\n", array_client->clients[i]->socket);
	}

	array_client_delete(array_client, sock);

	// DEBUG
	printf("client parti\n Array après : \n");
	for(i = 0; i < array_client->count; i++) {
		printf("elt %d\n", array_client->clients[i]->socket);
	}

	// closing the socket
	printf("closing the socket %d\n", sock);
	close(sock);

}
