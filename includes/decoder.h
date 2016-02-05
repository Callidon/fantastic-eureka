/*
 * Structures et fonctions relatives aux decodage des messages par le serveur
 * Auteurs : Pierre Gaultier & Thomas Minier
 */

/*TODO: TEST / MODIF */

/*
message :
code;datas

0 = broadcast => texte
1 = login => username;password
2 = leave => username
3 = msg => username;texte;
4 = mp => username;destinataire;texte

contraintes sur la taille du username/destinataire, du password et du message (max 250)

Exemple de message : 1;Pierre;123456  3;Thomas;Bonjour
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef DECODER_H
#define DECODER_H

// Structure représentant un message une fois parsé
typedef struct {
	int code;
	char *text;
	char *username;
	char *password;
	char *destinataire;
} message_parsed_t;

/*
 * Methode qui renvoie une structure a partir d'un message afin de le traiter dans le serveur
 */
message_parsed_t * decode(char* msg);

// TODO réparer le parseur
// TODO créer des fonctions pour générer les différents types de messages

#endif
