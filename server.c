/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h> 	/* pour les sockets */
#include <sys/socket.h>
#include <netdb.h> 		/* pour hostent, servent */
#include <string.h> 		/* pour bcopy, ... */
#include <signal.h>
#include "handler.h"
#define TAILLE_MAX_NOM 256

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

array_client_t * array_client;

// Fonction d'arrêt du serveur
void stop() {
	array_client_free(array_client);
	exit(0);
}

/*------------------------------------------------------*/
main(int argc, char **argv) {

    int socket_descriptor, 		/* descripteur de socket */
		nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
		longueur_adresse_courante; 	/* longueur d'adresse courante d'un client */
    sockaddr_in adresse_locale, 		/* structure d'adresse locale*/
	adresse_client_courant; 	/* adresse client courant */
    hostent* ptr_hote; 			/* les infos recuperees sur la machine hote */
    servent* ptr_service; 			/* les infos recuperees sur le service de la machine */
    char machine[TAILLE_MAX_NOM+1]; 	/* nom de la machine locale */

	// bind du CTRl+C pour arrêter proprement le serveur
	signal(SIGINT, stop);

	array_client = malloc(sizeof(array_client_t));
	array_client_init(array_client, 5);

    gethostname(machine,TAILLE_MAX_NOM);		/* recuperation du nom de la machine */

    /* recuperation de la structure d'adresse en utilisant le nom */
    if ((ptr_hote = gethostbyname(machine)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son nom.");
		exit(1);
    }

    /* initialisation de la structure adresse_locale avec les infos recuperees */

    /* copie de ptr_hote vers adresse_locale */
    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
    adresse_locale.sin_family		= ptr_hote->h_addrtype; 	/* ou AF_INET */
    adresse_locale.sin_addr.s_addr	= INADDR_ANY; 			/* ou AF_INET */
    adresse_locale.sin_port = htons(5001);

    printf("numero de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));

    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erreur : impossible de creer la socket de connexion avec le client.");
		exit(1);
    }

    /* association du socket socket_descriptor à la structure d'adresse adresse_locale */
    if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("Erreur : impossible de lier la socket a l'adresse de connexion.");
		exit(1);
    }

    /* initialisation de la file d'ecoute */
    listen(socket_descriptor,5);

    /* attente des connexions et traitement des donnees recues */
    for(;;) {

		longueur_adresse_courante = sizeof(adresse_client_courant);

		/* adresse_client_courant sera renseigné par accept via les infos du connect */
		if ((nouv_socket_descriptor =
			accept(socket_descriptor,
			       (sockaddr*)(&adresse_client_courant),
			       &longueur_adresse_courante))
			 < 0) {
			perror("erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}
		// TODO à supprimer
		printf("DEBUG - nouveau client connecté\n");

		// création du nouveau client
		int client_ind = array_client_add(array_client, nouv_socket_descriptor);
		client_datas_t * datas = malloc(sizeof(client_datas_t));
		datas->socket = nouv_socket_descriptor;
		datas->array_client = array_client;

		// lancement du handler chargé de gérer les interactions du nouveau client
		if(pthread_create(&array_client->clients[client_ind]->client_thread, NULL, server_handler, (void *) datas)) {
			perror("Erreur : Impossible de créer le thread pour le handler du client");
			exit(1);
		}

		// on signale au client qu'il est connecté
		char * login_msg;
		// formattage d'un message de type 1
		write(nouv_socket_descriptor, login_msg, strlen(login_msg));

		// multicast aux autres users pour leur signaler l'arrivée du nouvel user
		int i;
		for(i = 0; i < array_client->count; i++) {
			char * multicast_msg;
			// formattage d'un client de type 0
			write(array_client->clients[i]->socket, multicast_msg, strlen(multicast_msg));
		}
    }

}
