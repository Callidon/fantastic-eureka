/*-----------------------------------------------------------
Client a lancer apres le serveur avec la commande :
client <adresse-serveur> <message-a-transmettre>
------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "config.h"
#include "handler.h"

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

pthread_t thread_handler; /* Thread du handler côté client */
int socket_descriptor; /* descripteur de socket */
render_datas_t * render_datas; /* données à passer au thread de rendu */

int main(int argc, char **argv) {

    int longueur,			/* longueur d'un buffer utilisé */
		successful_login = 0;	/* Booléen indiquant si l'authentifgication s'est faite avec succès */
    sockaddr_in adresse_locale; 	/* adresse de socket local */
    hostent *	ptr_host; 		/* info sur une machine hote */
    servent *	ptr_service; 		/* info sur service */
    char buffer[MAX_BUFFER_SIZE],	/* buffer de réception des messages venant du serveur */
		message[MAX_BUFFER_SIZE],	/* message envoyé */
		username[MAX_USERNAME_SIZE], /* nom d'utilisateur */
		password[MAX_PASSWORD_SIZE], /* Mot de passe */
		destinataire[MAX_USERNAME_SIZE]; /* Nom du destinataire d'un message privé */
	char * action;	/* Action désirée */
    char *	host;	/* nom de la machine distante */
	WINDOW 	* wchat, /* Pointeur vers la fenêtre d'affichage du chat */
			* winput; /* Pointeur vers la fenêtre d'es inputs utilisateurs */

	memset(buffer, 0, MAX_BUFFER_SIZE);
	memset(message, 0, MAX_BUFFER_SIZE);
	memset(username, 0, MAX_USERNAME_SIZE);

	// vérification et récupération des arguments
    if (argc != 2) {
		perror("usage : client <adresse-serveur>");
		exit(1);
    }

    host = argv[1];

    printf("adresse du serveur  : %s \n", host);

    if ((ptr_host = gethostbyname(host)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son adresse.");
		exit(1);
    }

    /* copie caractere par caractere des infos de ptr_host vers adresse_locale */
    bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
    adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype; */

    adresse_locale.sin_port = htons(5001);

    printf("numero de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));

    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("erreur : impossible de creer la socket de connexion avec le serveur.");
		exit(1);
    }

    /* tentative de connexion au serveur dont les infos sont dans adresse_locale */
    if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("erreur : impossible de se connecter au serveur.");
		exit(1);
    }

	// passage en mode ncurses
	initscr();

	wchat = subwin(stdscr, LINES/2, COLS, 0, 0);
	winput = subwin(stdscr, LINES/2, COLS, LINES/2, 0);
	scrollok(wchat, TRUE);
	scrollok(winput, TRUE);

	wrefresh(wchat);
	wrefresh(winput);

	// stockage des données et lancement du thread pour le rendu du chat
	render_datas = malloc(sizeof(render_datas_t));
	render_datas->window = wchat;
	render_datas->socket = socket_descriptor;
	render_datas->successful_login = &successful_login;
	pthread_mutex_init(&render_datas->login_mutex, NULL);
	if(pthread_create(&thread_handler, NULL, client_handler, render_datas) < 0) {
		perror("erreur : impossible de créer un nouveau thread");
		exit(1);
	}

	while(! successful_login) {
		// on lock le mutex lié au login
		pthread_mutex_lock(&render_datas->login_mutex);

		// demande de l'username & du password tant que l'on n'est pas connecté
		menu_ask_username(winput, username);
		wrefresh(wchat);
		menu_ask_password(winput, password);
		wrefresh(wchat);

		// envoi un message de type login au serveur (password écrit en dur pour l'instant)
		generateLogin(message, username, password);
		write(socket_descriptor, message, strlen(message) + 1);
		clear_window(winput);

		// on attend une modif sur le booléen successful_login
		pthread_mutex_lock(&render_datas->login_mutex);

		if(! successful_login) {
			wprintw(winput, "Erreur : mot de passe incorrect. Veuillez essayer à nouveau\n");
			wrefresh(winput);
		}
	}

	wprintw(wchat, "Connecté en tant que %s\n", username);
	wrefresh(wchat);
	// sélection d'action :
	// demande d'input selon un numéro d'un menu
	for(;;) {
		wprintw(winput, "Tapez le numéro correspondant à l'action désirée :\n");
		wprintw(winput, "1 : Envoyer un message\n2 : Envoyer un message privé\n3 : Déconnexion\n");
		wprintw(winput, "Action : ");
		wrefresh(winput);
		// en fonction du numéro
		switch(wgetch(winput)) {
			// cas message
			case 49 : {
				wprintw(winput, "\n");
				menu_say(winput, buffer);
				wrefresh(winput);
				// envoi du message au serveur
				generateMsg(message, username, buffer);
				write(socket_descriptor, message, strlen(message) + 1);
			}
				break;
			// cas whisper :
			case 50 : {
				wprintw(winput, "\n");
				menu_whisper(winput, destinataire, buffer);
				wrefresh(winput);
				// envoi du message au serveur
				generateWhisp(message, username, destinataire, buffer);
				write(socket_descriptor, message, strlen(message) + 1);
			}
				break;
			// cas de déconnexion :
			case 51 : {
				generateLeave(message, "User wants to leave the channel");
				write(socket_descriptor, message, strlen(message) + 1);

				// on attend la fin du thread du handler avant de fermer le programme
				pthread_join(thread_handler, NULL);

				// fin du programme
				endwin();
				delwin(wchat);
				delwin(winput);
				exit(0);
			}
				break;
			default:{
				wprintw(winput, "\n");
				menu_say(winput, buffer);
				wprintw(winput, "Action inconnue\n");
			}
		}
		clear_window(winput);
	}
}
