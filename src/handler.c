/*
 * Structures et fonctions relatives à la réception et au traitement des messages réseaux
 * Auteurs : Pierre Gaultier & Thomas Minier
 */
#include "handler.h"

/*
 * Méthode qui gère la réception et le traitement des messages reçus côté serveur
 */
void * server_handler(void * client_datas) {
	client_datas_t * datas = (client_datas_t *) client_datas;
	message_parsed_t * message;
    char buffer[MAX_BUFFER_SIZE];
	char response[MAX_BUFFER_SIZE];
	char temp_buffer[MAX_BUFFER_SIZE];
    int i,
		longueur;

	memset(buffer, 0, MAX_BUFFER_SIZE);
	memset(response, 0, MAX_BUFFER_SIZE);
	memset(temp_buffer, 0, MAX_BUFFER_SIZE);

	for(;;) {

		/* lecture du message rentrant */
	    if ((longueur = read(datas->client->socket, buffer, sizeof(buffer))) <= 0) {
		    perror("Erreur : impossible de lire le message entrant");
			pthread_exit(0);
		}

		/*on décode le message */
		message = decode(buffer);

		switch(message->code) {
			/* cas d'un message de multicast */
			case Multicast : {
				if(datas->client->is_logged) {
					/* on transmet le message à chaque client */
					for(i = 0; i < datas->array_client->count; i++) {
						write(datas->array_client->clients[i]->socket, message->text, strlen(message->text) + 1);
					}
				}
			}
				break;
			/* cas d'un message de connexion */
			case Login: {
				if(! datas->client->is_logged) {
					/* si le mot de passe est correct */
					if(strcmp(message->password, datas->server_password) == 0) {
						/* set de l'username du client */
						array_client_setName(datas->array_client, datas->client->socket, message->username);
						memcpy(datas->client->username, message->username, strlen(message->username) + 1);

						/* envoi d'un message de type 1 au client pour valider l'échange */
						generateAckLogin(response, 1);
						write(datas->client->socket, response, strlen(response) + 1);

						/* multicast aux autres users pour leur signaler l'arrivée du nouvel user */
						concat(temp_buffer, message->username, " has join the channel");
						memset(response, 0, MAX_BUFFER_SIZE);
						generateMulticast(response, temp_buffer);
						for(i = 0; i < datas->array_client->count; i++) {
							if(datas->array_client->clients[i]->socket != datas->client->socket) {
								write(datas->array_client->clients[i]->socket, response, strlen(response) + 1);
							}
						}

						/* on signale que le client est identifié */
						datas->client->is_logged = 1;
					} else {
						/* envoi d'un message d'erreur au client */
						generateAckLogin(response, 0);
						write(datas->client->socket, response, strlen(response) + 1);
					}
				}
			}
				break;
			/* cas d'un message de déconnexion */
			case Leave : {
				if(datas->client->is_logged) {
					/* on signale au client q'il peut terminer la connexion de son côté */
					generateLeave(response, "user can leave the channel");
					write(datas->client->socket, response, strlen(response) + 1);

					/* multicast d'un message pour annoncer le départ du client */
					concat(temp_buffer, message->username, " has leave the channel");
					memset(response, 0, MAX_BUFFER_SIZE);
					generateMulticast(response, temp_buffer);

					for(i = 0; i < datas->array_client->count; i++) {
						if(datas->array_client->clients[i]->socket != datas->client->socket) {
							write(datas->array_client->clients[i]->socket, response, strlen(response) + 1);
						}
					}

					/* on termine la connexion avec le client */
					close(datas->client->socket);
					array_client_delete(datas->array_client, datas->client->socket);

					free(datas);
					pthread_exit(0);
				}
			}
				break;
			/* cas d'un message classique */
			case Say : {
				if(datas->client->is_logged) {
					/* transmission du message aux autres clients */
					generateMsg(response, message->username, message->text);
					for(i = 0; i < datas->array_client->count; i++) {
						write(datas->array_client->clients[i]->socket, response, strlen(response) + 1);
					}
				}
			}
				break;
			/*  cas d'un message de privé */
			case Whisper : {
				if(datas->client->is_logged) {
					/* recherche du destinataire via son username */
					client_t * destinataire = NULL;
					for(i = 0; i < datas->array_client->count; i++) {
						if(strcmp(datas->array_client->clients[i]->username, message->destinataire) == 0) {
							destinataire = datas->array_client->clients[i];
							break;
						}
					}
					/* envoi du message au destinataire si ce dernier existe */
					if(destinataire != NULL) {
						generateWhisp(response, message->username, message->destinataire, message->text);
						write(destinataire->socket, response, strlen(response) + 1);
					} else {
						/* envoi d'un message d'erreur au client */
						generateMulticast(response, "Erreur , le destinataire en question n'existe pas");
						write(datas->client->socket, response, strlen(response) + 1);
					}
				}
			}
				break;
			default : {
				printf("Warning : message entrant non reconnu par le protocole : %s\n", buffer);
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
	message_parsed_t * message;
    char buffer[MAX_BUFFER_SIZE];
	char response[MAX_BUFFER_SIZE];
	int longueur;

	memset(buffer, 0, MAX_BUFFER_SIZE);
	memset(response, 0, MAX_BUFFER_SIZE);

	for(;;) {

		/* lecture du message entrant */
	    if ((longueur = read(datas->socket, buffer, sizeof(buffer))) <= 0) {
		    perror("Erreur : impossible de lire le message entrant");
			pthread_exit(0);
		}

		/* on décode le message */
		message = decode(buffer);

		switch(message->code) {
			/* cas d'un message de multicast */
			case Multicast : {
				print_multicast(datas->window, message->text);
			}
				break;
			/* cas d'un message de déconnexion */
			case Leave : {
				print_multicast(datas->window, "Leaving the channel...");
				close(datas->socket);
				free(datas);
				pthread_exit(0);
			}
				break;
			/* cas d'un message classique */
			case Say : {
				print_message(datas->window, message->username, message->text);
			}
				break;
			/* cas d'un message privé */
			case Whisper : {
				print_whisper(datas->window, message->username, message->text);
			}
				break;
			/* cas d'un message d'accusé de connexion */
			case AckLogin : {
				if(atoi(message->text) == 1) {
					write(datas->fileDescriptor, message->text, strlen(message->text) + 1);
				} else {
					write(datas->fileDescriptor, "0", 2);
				}
			}
				break;
			default : {
				printf("Warning : message entrant non reconnu par le protocole : %s\n", buffer);
			}
				break;
		}
		wrefresh(datas->window);
		message_parsed_free(message);
	}
}
