#include <stdio.h>
#include "handler.h"

void * server_handler(void * client_datas) {
	client_datas_t * datas = (client_datas_t *) client_datas;

    char buffer[250];
		char * msg;
    int longueur;
		int client_ind;


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
				int i;
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
				int i;
				for(i = 0; i < datas->array_client->count; i++) {
					// formattage du message de type 0
					write(datas->array_client->clients[i]->socket, msg, strlen(msg));
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
				int i;
				for(i = 0; i < datas->array_client->count; i++) {
					// formattage du message de type 0
					write(datas->array_client->clients[i]->socket, msg, strlen(msg));
				}
				break;
			// message
			case 3 :
				// TODO à supprimer
				printf("DEBUG message - message lu : %s \n", buffer);
				// transmission du message aux autres clients
				int i;
				for(i = 0; i < datas->array_client->count; i++) {
					// formattage du message de type 3
					write(datas->array_client->clients[i]->socket, msg, strlen(msg));
				}
				break;
			//  message privé
			case 4 :
				// TODO à supprimer
				printf("DEBUG mp - message lu : %s \n", buffer);
				// recherche du destinataire via son username
				int i;
				client_t * destinataire;
				while(i = 0; i < datas->array_client->count; i++) {
					if(strcmp(datas->array_client->clients[i]->username, /*message->username*/)) {
						destinataire = datas->array_client->clients[i];
						break;
					}
				}
				// envoi du message au destinataire
				// formattage d'un message de type 4
				write(destinataire->socket, msg, strlen(msg));
				break;
		}
		// appel du destructeur sur message
	}
}
