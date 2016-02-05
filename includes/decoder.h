/*
 * Structures et fonctions relatives aux decodage des messages par le serveur
 * Auteurs : Pierre Gaultier & Thomas Minier
 */


/*TODO: TEST / MODIF */


/*
message :
code:datas

0 = broadcast => texte
1 = login => username;password
2 = leave => username
3 = msg => username;texte;
4 = mp => username;destinataire;texte

Exemple de message : 1;Pierre;123456  3;Thomas;Bonjour
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef DECODER_H
#define DECODER_H

// Structure représentant un message une fois parsé
typedef struct {
	char *code;
	char *message;
	char *username;
	char *password;
	char *destinataire;
} message_parsed;


/*
 * Methode qui renvoie une structure a partir d'un message afin de le traiter dans le serveur
 */
message_parsed *decode(char* msg);


#endif
