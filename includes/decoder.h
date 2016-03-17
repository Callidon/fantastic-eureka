/*
 * Structures et fonctions relatives aux decodage des messages par le serveur
 * Auteurs : Pierre Gaultier & Thomas Minier
 */

#ifndef DECODER_H
#define DECODER_H

#include "config.h"
#include <stdlib.h>
#include <string.h>

/* Enum représentant le type d'un message */
typedef enum { Multicast, Login, Leave, Say, Whisper, AckLogin } MSGType;

/* Structure représentant un message une fois parsé */
typedef struct {
  MSGType code;
  char *text;
  char *username;
  char *password;
  char *destinataire;
} message_parsed_t;

/*
 * Fonction de concaténation de deux chaînes de caractères
 */
void concat(char *buffer, char *left, char *right);

/*
 * Methode qui renvoie une structure a partir d'un message afin de le traiter
 * dans le serveur
 */
message_parsed_t *decode(char *msg);

/*
 * Methode qui renvoie un message multicast sous forme de chaine a partir des
 * parametres remplis par le client
 */
void generateMulticast(char *resp, char *msg);

/*
 * Methode qui renvoie un message de login sous forme de chaine a partir des
 * parametres remplis par le client
 */
void generateLogin(char *resp, char *username, char *password);

/*
 * Methode qui renvoie un message de deconnexionsous forme de chaine a partir
 * des parametres remplis par le client
 */
void generateLeave(char *resp, char *username);

/*
 * Methode qui renvoie un message pour tout les autres clients sous forme de
 * chaine a partir des parametres remplis par le client
 */
void generateMsg(char *resp, char *username, char *msg);

/*
 * Methode qui renvoie un message privé pour un autre client sous forme de
 * chaine a partir des parametres remplis par le client
 */
void generateWhisp(char *resp, char *username, char *destinataire, char *msg);

/*
 * Methode qui renvoie un aquittement quand le client a essayé de se connecter
 */
void generateAckLogin(char *resp, int code);

/*
 * Méthode libérant une structure message_parsed_t
 */
void message_parsed_free(message_parsed_t *message);

#endif
