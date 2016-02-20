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
#include <signal.h>
#include "config.h"
#include "handler.h"

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

pthread_t thread_handler; /* Thread du handler côté client */
int socket_descriptor; /* descripteur de socket */
render_datas_t * render_datas; /* données à passer au thread de rendu */

// Fonction d'arrêt du client
void stop() {
	pthread_cancel(thread_handler);
	free(render_datas);
	// fin de la réception
    close(socket_descriptor);
	// fin de ncruses
	// TODO
	exit(0);
}

// Fonction de saisie de texte pour un message
void text_input(WINDOW *win, char * message, size_t max_char) {
	wgetnstr(win, message, max_char);
	// remplacement du retour chariot par un terminateur
	char *p = strchr(message, '\n');
    if(p) {
		*p = '\0';
	}
}

int main(int argc, char **argv) {

    int longueur; 		/* longueur d'un buffer utilisé */
    sockaddr_in adresse_locale; 	/* adresse de socket local */
    hostent *	ptr_host; 		/* info sur une machine hote */
    servent *	ptr_service; 		/* info sur service */
    char buffer[MAX_BUFFER_SIZE];	/* buffer de réception des messages venant du serveur */
	char message[MAX_BUFFER_SIZE];	/* message envoyé */
    char *	prog; 			/* nom du programme */
    char *	host; 			/* nom de la machine distante */
	WINDOW * wchat, * winput; /* Fenêtres d'affichage */

	memset(buffer, 0, MAX_BUFFER_SIZE);
	memset(message, 0, MAX_BUFFER_SIZE);

    if (argc != 2) {
		perror("usage : client <adresse-serveur>");
		exit(1);
    }

    prog = argv[0];
    host = argv[1];

	// bind du CTRl+C pour arrêter proprement le client
	signal(SIGINT, stop);

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

	// init de la fenête d'input TODO
	mvwprintw(winput, 1, 1, "Message : ");
	wrefresh(wchat);
	wrefresh(winput);

	// stockage des données et lancement du thread pour le rendu du chat
	render_datas = malloc(sizeof(render_datas_t));
	render_datas->window = wchat;
	render_datas->socket = socket_descriptor;
	if(pthread_create(&thread_handler, NULL, client_handler, (void *) render_datas) < 0) {
		perror("erreur : impossible de créer un nouveau thread");
		exit(1);
	}

	// input
	text_input(winput, message, MAX_BUFFER_SIZE);
	// affichag de l'input
	wprintw(wchat, "message tapé : %s\n", message);
	wrefresh(wchat);
	// resresh de la fenêtre d'input
	wclear(winput);
	mvwprintw(winput, 1, 1, "Message : ");
	wrefresh(winput);
	// délai pour visualiser les changements
	sleep(5);

	// création du message de départ du chat
	generateLeave(message, "User has leave the channel");
	write(socket_descriptor, message, strlen(message));

	// fin du programme
	endwin();
	delwin(wchat);
	delwin(winput);
	stop();
}
