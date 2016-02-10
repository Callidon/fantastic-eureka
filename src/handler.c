#include <stdio.h>
#include "handler.h"

void * server_handler(void * client_datas) {
	client_datas_t * datas = (client_datas_t *) client_datas;
    char buffer[250];
	char * response;
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
			case 0 :
				// TODO à supprimer
				printf("DEBUG multicast - message lu : %s \n", buffer);
				// on transmet le message à chaque client
				for(i = 0; i < datas->array_client->count; i++) {
					write(datas->array_client->clients[i]->socket, buffer, strlen(buffer));
				}
				break;
			// login
			case 1 :
				// TODO à supprimer
				printf("DEBUG login - message lu : %s \n", buffer);
				client_ind = array_client_setName(datas->array_client, datas->socket, message->username);
				// multicast d'un message pour annoncer la connexion du client
				response = generateMulticast(strcat(message->username, " has join the channel"));
				for(i = 0; i < datas->array_client->count; i++) {
					if(datas->array_client->clients[i]->socket != datas->socket) {
						write(datas->array_client->clients[i]->socket, response, strlen(response));
					}
				}
				break;
			// leave
			case 2 :
				// TODO à supprimer
				printf("DEBUG leave - message lu : %s \n", buffer);
				array_client_delete(datas->array_client, datas->socket);
				close(datas->socket);
				free(datas);

				// multicast d'un message pour annoncer le départ du client
				response = generateMulticast(strcat(message->username, " has leave the channel"));
				for(i = 0; i < datas->array_client->count; i++) {
					if(datas->array_client->clients[i]->socket != datas->socket) {
						write(datas->array_client->clients[i]->socket, response, strlen(response));
					}
				}
				break;
			// message
			case 3 :
				// TODO à supprimer
				printf("DEBUG message - message lu : %s \n", buffer);
				// transmission du message aux autres clients
				response = generateMsg(message->username, message->text);
				for(i = 0; i < datas->array_client->count; i++) {
					if(datas->array_client->clients[i]->socket != datas->socket) {
						write(datas->array_client->clients[i]->socket, response, strlen(response));
					}
				}
				break;
			//  message privé
			case 4 :
				// TODO à supprimer
				printf("DEBUG mp - message lu : %s \n", buffer);
				// recherche du destinataire via son username
				client_t * destinataire;
				for(i = 0; i < datas->array_client->count; i++) {
					if(strcmp(datas->array_client->clients[i]->username, message->username)) {
						destinataire = datas->array_client->clients[i];
						break;
					}
				}
				// envoi du message au destinataire
				response = generateWhisp(message->username, message->destinataire, message->text);
				write(destinataire->socket, response, strlen(response));
				break;
		}
		// nettoyage des variables
		free(response);
		message_parsed_free(message);
	}
}
