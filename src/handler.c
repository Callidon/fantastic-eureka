#include <stdio.h>
#include "handler.h"

void * server_handler(void * client_datas) {
	client_datas_t * datas = (client_datas_t *) client_datas;

    char buffer[250];
    int longueur;
	int client_ind;

	// TODO mettre le décodage dans un for(;;)
	// TODO coder les différents cas

	// lecture du message
    if ((longueur = read(datas->socket, buffer, sizeof(buffer))) <= 0) {
	    return;
	}

	// on décode le message
	message_parsed_t * message = decode(buffer);

	switch(message->code) {
		// broadcast
		case 0 :
			// for dans datas->array_clients
				// write du message dans le socket courant
			// TODO à supprimer
			printf("message lu : %s \n", buffer);

			buffer[0] = 'R';
			buffer[1] = 'E';
			buffer[longueur] = '#';
			buffer[longueur+1] ='\0';

			printf("message apres traitement : %s \n", buffer);

			printf("renvoi du message traite.\n");

			sleep(3);

			write(datas->socket,buffer,strlen(buffer)+1);

			printf("message envoye. \n");
			break;
		// login
		case 1 :
			client_ind = array_client_setName(datas->array_client, datas->socket, message->username);
			// broadcast d'un message pour annoncer la connexion du client
			// for dans datas->array_clients
				// write du message de type 0 dans le socket courant
			break;
		// leave
		case 2 :
			array_client_delete(datas->array_client, datas->socket);
			close(datas->socket);
			free(datas);
			// broadcast d'un message pour annoncer le départ du client
			// for dans datas->array_clients
				// write du message de type 0 dans le socket courant
			break;
		// message send
		case 3 :
			// transmission du message aux autres clients
			// for dans datas->array_clients
				// write du message de type 3 dans le socket courant
			break;
		//  message privé
		case 4 :
			// recherche du destinataire via son username
			// write du message de type 4 dans le socket de l'username
			break;
	}
	// TODO à supprimer
	array_client_delete(datas->array_client, datas->socket);
	close(datas->socket);
	free(datas);
}
