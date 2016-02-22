#include <stdio.h>
#include "handler.h"

/*
 * Méthode qui gère la réception et le traitement des messages reçus côté serveur
 */
void * server_handler(void * client_datas) {
	client_datas_t * datas = (client_datas_t *) client_datas;
    char buffer[MAX_BUFFER_SIZE];
	char response[MAX_BUFFER_SIZE];
    int i,
		longueur,
		client_ind;

	for(;;) {
		memset(buffer, 0, MAX_BUFFER_SIZE);
		memset(response, 0, MAX_BUFFER_SIZE);

		// lecture du message rentrant
	    if ((longueur = read(datas->socket, buffer, sizeof(buffer))) <= 0) {
		    return;
		}

		// TODO à supprimer
		printf("DEBUG - message lu : %s \n", buffer);

		// on décode le message
		message_parsed_t * message = decode(buffer);

		switch(message->code) {
			// cas d'un message de multicast
			case Multicast : {
				// on transmet le message à chaque client
				for(i = 0; i < datas->array_client->count; i++) {
					write(datas->array_client->clients[i]->socket, message->text, sizeof(message->text));
				}
			}
				break;
			// cas d'un message de connexion
			case Login: {
				// set de l'username du client
				array_client_setName(datas->array_client, datas->socket, message->username);

				// envoi d'un message de type 1 au client pour valider l'échange
				generateAckLogin(response);
				write(datas->socket, response, strlen(response));
				memset(response, 0, MAX_BUFFER_SIZE);
				// multicast aux autres users pour leur signaler l'arrivée du nouvel user
				generateMulticast(response, "user has join the channel");
				for(i = 0; i < datas->array_client->count; i++) {
					if(datas->array_client->clients[i]->socket != datas->socket) {
						write(datas->array_client->clients[i]->socket, response, strlen(response));
					}
				}

			}
				break;
			// cas d'un message de déconnexion
			case Leave : {
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
				pthread_exit(0);
			}
				break;
			// cas d'un message classique
			case Say : {
				// transmission du message aux autres clients
				generateMsg(response, message->username, message->text);
				for(i = 0; i < datas->array_client->count; i++) {
					if(datas->array_client->clients[i]->socket != datas->socket) {
						write(datas->array_client->clients[i]->socket, response, strlen(response));
					}
				}
			}
				break;
			//  cas d'un message de privé
			case Whisper : {
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
			}
				break;
		}
		message_parsed_free(message);
	}
}

/*
 * Méthode qui gère la réception et le traitement des messages reçus côté client
 */
void * client_handler(void * render_datas) {
	render_datas_t * datas = (render_datas_t *) render_datas;
    char buffer[MAX_BUFFER_SIZE];
	char response[MAX_BUFFER_SIZE];
    int i,
		longueur;

	for(;;) {
		memset(buffer, 0, MAX_BUFFER_SIZE);
		memset(response, 0, MAX_BUFFER_SIZE);

		// lecture du message entrant
	    if ((longueur = read(datas->socket, buffer, sizeof(buffer))) <= 0) {
		    return;
		}
		// TODO à supprimer
		wprintw(datas->window, "DEBUG - message lu : %s \n", buffer);

		// on décode le message
		message_parsed_t * message = decode(buffer);

		switch(message->code) {
			// cas d'un message de multicast
			case Multicast : {
				// on affiche le message
				wprintw(datas->window, message->text);
			}
				break;
			// cas d'un message de déconnexion
			case Leave : {
				wprintw(datas->window, "leave"); // FIX ME
			}
				break;
			// cas d'un message classique
			case Say : {
				wprintw(datas->window, "say"); // FIX ME
			}
				break;
			//  cas d'un message privé
			case Whisper : {
				wprintw(datas->window, "whisper"); // FIX ME
			}
				break;
		}
		wrefresh(datas->window);
		message_parsed_free(message);
	}
}
