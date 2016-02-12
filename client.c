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

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

int socket_descriptor; /* descripteur de socket */

// Fonction d'arrêt du client
void stop() {
	printf("\nfin de la reception.\n");
    close(socket_descriptor);
    printf("connexion avec le serveur fermee, fin du programme.\n");
}

// Fonction de saisie de texte pour un message
void text_input(char * message) {
	fgets(message, sizeof(message), stdin);
	// suppresion d'un éventuel retour chariot
	char *p = strchr(message, '\n');
    if(p) {
		*p = 0;
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


    /* lecture des messages en provenance du serveur */
    while((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
		printf("message reçu du serveur : %s\n", buffer);
		printf("Saisissez votre message :\n");
		text_input(message);
		/* envoi du message vers le serveur */
		if ((write(socket_descriptor, message, strlen(message))) < 0) {
			perror("erreur : impossible d'ecrire le message destine au serveur.");
			exit(1);
		}
    }
	exit(0);
}
