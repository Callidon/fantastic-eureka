#include <stdio.h>
#include "handler.h"

void * server_handler(void * client_datas) {
	client_datas_t * datas = (client_datas_t *) client_datas;
    char buffer[MAX_BUFFER_SIZE];
	char response[MAX_BUFFER_SIZE];
    int i,
		longueur,
		client_ind;

	for(;;) {
		// lecture du message
	    if ((longueur = read(datas->socket, buffer, sizeof(buffer))) <= 0) {
		    return;
		}
		printf("message lu : %s \n", buffer);

		// on décode le message
		message_parsed_t * message = decode(buffer);

		switch(message->code) {
			// multicast
			case Multicast :
				// on transmet le message à chaque client
				for(i = 0; i < datas->array_client->count; i++) {
					write(datas->array_client->clients[i]->socket, message->text, sizeof(message->text));
				}
				break;
			// leave
			case Leave :
				array_client_delete(datas->array_client, datas->socket);
				close(datas->socket);

				// multicast d'un message pour annoncer le départ du client
				generateMulticast(response, "user has leave the channel");
				for(i = 0; i < datas->array_client->count; i++) {
					if(datas->array_client->clients[i]->socket != datas->socket) {
						write(datas->array_client->clients[i]->socket, response, strlen(response));
					}
				}
				free(datas);
				exit(1);
				break;
			// message
			case Say :
				// transmission du message aux autres clients
				generateMsg(response, message->username, message->text);
				for(i = 0; i < datas->array_client->count; i++) {
					if(datas->array_client->clients[i]->socket != datas->socket) {
						write(datas->array_client->clients[i]->socket, response, strlen(response));
					}
				}
				break;
			//  message privé
			case Whisper :
				// recherche du destinataire via son username
				client_t * destinataire;
				for(i = 0; i < datas->array_client->count; i++) {
					if(strcmp(datas->array_client->clients[i]->username, message->destinataire)) {
						destinataire = datas->array_client->clients[i];
						break;
					}
				}
				// envoi du message au destinataire
				generateWhisp(response, message->username, message->destinataire, message->text);
				write(destinataire->socket, response, strlen(response));
				break;
		}
		// nettoyage des variables
		message_parsed_free(message);
	}
}
