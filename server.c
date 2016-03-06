/*
 * Serveur à lancer pour créer un salon de discussion
 * usage : ./serveur <mot-de-passe>
 * Auteurs : Pierre Gaultier & Thomas Minier
 */
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include "config.h"
#include "handler.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

/* Collection des clients connectés au serveur */
array_client_t * array_client;

/* Fonction d'arrêt normale du serveur */
void stop() {
	array_client_free(array_client);
	exit(0);
}

/* Fonction d'arrêt du serveur en cas d'erreur */
void error_stop() {
	array_client_free(array_client);
	exit(1);
}

/*------------------------------------------------------*/
int main(int argc, char **argv) {

    int socket_descriptor, 		/* descripteur de socket */
		nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
		client_ind;		/* indice d'un client dans une collection de clients */
	socklen_t longueur_adresse_courante;	/* longueur d'adresse courante d'un client */
	sockaddr_in adresse_locale, 	/* structure d'adresse locale*/
				adresse_client_courant; 	/* adresse client courant */
    hostent* ptr_hote; 			/* les infos recuperees sur la machine hote */
    char machine[MAX_BUFFER_SIZE+1], 	/* nom de la machine locale */
		 * password;	/* Mot de passe du serveur */
	client_datas_t * datas; /* Données relatives à un client tramise à un thread */

	if (argc != 2) {
		perror("usage : server <mot-de-passe-serveur>");
		exit(1);
    }

    password = argv[1];

	/* bind du CTRl+C pour arrêter proprement le serveur */
	signal(SIGINT, stop);

	/* Initialisation de la collection de clients */
	array_client = malloc(sizeof(array_client_t));
	array_client_init(array_client, 5);

	/* récuperation du nom de la machine */
    gethostname(machine,MAX_BUFFER_SIZE);

    /* récuperation de la structure d'adresse en utilisant le nom */
    if ((ptr_hote = gethostbyname(machine)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son nom.");
		error_stop();
    }

    /* initialisation de la structure adresse_locale avec les infos recuperees */

    /* copie de ptr_hote vers adresse_locale */
    bcopy((char*) ptr_hote->h_addr, (char*) &adresse_locale.sin_addr, ptr_hote->h_length);
    adresse_locale.sin_family		= ptr_hote->h_addrtype; 	/* ou AF_INET */
    adresse_locale.sin_addr.s_addr	= INADDR_ANY; 			/* ou AF_INET */
    adresse_locale.sin_port = htons(5001);

    printf("numero de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));

    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erreur : impossible de creer la socket de connexion avec le client");
		error_stop();
    }

	if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
		perror("Erreur : l'opération setsockopt(SO_REUSEADDR) a échouée");
		error_stop();
	}

    /* association du socket socket_descriptor à la structure d'adresse adresse_locale */
    if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("Erreur : impossible de lier la socket a l'adresse de connexion");
		error_stop();
    }

    /* initialisation de la file d'ecoute */
    listen(socket_descriptor, 5);

    /* attente des connexions et traitement des donnees recues */
    for(;;) {

		longueur_adresse_courante = sizeof(adresse_client_courant);

		/* adresse_client_courant sera renseigné par accept via les infos du connect */
		if ((nouv_socket_descriptor =
			accept(socket_descriptor,
			       (sockaddr*)(&adresse_client_courant),
			       &longueur_adresse_courante))
			 < 0) {
			perror("Erreur : impossible d'accepter la connexion avec le client");
			error_stop();
		}

		/* création du nouveau client */
		client_ind = array_client_add(array_client, nouv_socket_descriptor);
		datas = malloc(sizeof(client_datas_t));
		datas->client = array_client->clients[client_ind];
		datas->server_password = password;
		datas->array_client = array_client;

		/* lancement du handler chargé de gérer les interactions avec le nouveau client */
		if(pthread_create(&array_client->clients[client_ind]->client_thread, NULL, server_handler, (void *) datas) < 0) {
			perror("Erreur : impossible de créer le thread pour gérer la connexion avec le client");
			error_stop();
		}
		pthread_detach(array_client->clients[client_ind]->client_thread);
    }
}
