/*
 * Structures et fonctions relatives aux decodage des messages par le serveur
 * Auteurs : Pierre Gaultier & Thomas Minier
 */
#include "decoder.h"

/*
 * Fonction de concaténation de deux chaînes de caractères
 */
void concat(char *buffer, char *left, char *right) {
  memcpy(buffer, left, strlen(left));
  memcpy(buffer + strlen(left), right, strlen(right) + 1);
}

/*
* Methode qui renvoie une structure a partir d'un message afin de le traiter
* dans le serveur
*/
message_parsed_t *decode(char *msg) {
  message_parsed_t *msg_prd;
  char *token;

  token = strtok(msg, ";");
  msg_prd = malloc(sizeof(message_parsed_t));

  switch (atoi(token)) {
  /* Cas message de type Multicast */
  case 0:
    msg_prd->code = Multicast;
    token = strtok(NULL, ";");
    msg_prd->text = strdup(token);
    msg_prd->username = NULL;
    msg_prd->password = NULL;
    msg_prd->destinataire = NULL;
    break;
  /* Cas message de type Login */
  case 1:
    msg_prd->code = Login;
    token = strtok(NULL, ";");
    msg_prd->text = NULL;
    msg_prd->username = strdup(token);
    token = strtok(NULL, ";");
    msg_prd->password = strdup(token);
    msg_prd->destinataire = NULL;
    break;
  /* Cas message de type Leave */
  case 2:
    msg_prd->code = Leave;
    token = strtok(NULL, ";");
    msg_prd->text = NULL;
    msg_prd->username = strdup(token);
    msg_prd->password = NULL;
    msg_prd->destinataire = NULL;
    break;
  /* Cas message de type Say */
  case 3:
    msg_prd->code = Say;
    token = strtok(NULL, ";");
    msg_prd->username = strdup(token);
    token = strtok(NULL, ";");
    msg_prd->text = strdup(token);
    msg_prd->password = NULL;
    msg_prd->destinataire = NULL;
    break;
  /* Cas message de type Whisper */
  case 4:
    msg_prd->code = Whisper;
    token = strtok(NULL, ";");
    msg_prd->username = strdup(token);
    msg_prd->password = NULL;
    token = strtok(NULL, ";");
    msg_prd->destinataire = strdup(token);
    token = strtok(NULL, ";");
    msg_prd->text = strdup(token);
    break;
  /* Cas message de type AckLogin */
  case 5:
    msg_prd->code = AckLogin;
    token = strtok(NULL, ";");
    msg_prd->text = strdup(token);
    msg_prd->username = NULL;
    msg_prd->password = NULL;
    msg_prd->destinataire = NULL;
    break;
  }
  return msg_prd;
}

/*
* Methode qui renvoie un message multicast sous forme de chaine a partir des
* parametres remplis par le client
*/
void generateMulticast(char *resp, char *msg) {
  memcpy(resp, "0;", 2);
  memcpy(resp + 2, msg, strlen(msg) + 1);
}

/*
* Methode qui renvoie un message de login sous forme de chaine a partir des
* parametres remplis par le client
*/
void generateLogin(char *resp, char *username, char *password) {
  memcpy(resp, "1;", 2);
  memcpy(resp + 2, username, strlen(username));
  memcpy(resp + 2 + strlen(username), ";", 1);
  memcpy(resp + 2 + strlen(username) + 1, password, strlen(password) + 1);
}

/*
* Methode qui renvoie un message de deconnexionsous forme de chaine a partir des
* parametres remplis par le client
*/
void generateLeave(char *resp, char *username) {
  memcpy(resp, "2;", 2);
  memcpy(resp + 2, username, strlen(username) + 1);
}

/*
* Methode qui renvoie un message pour tout les autres clients sous forme de
* chaine a partir des parametres remplis par le client
*/
void generateMsg(char *resp, char *username, char *msg) {
  memcpy(resp, "3;", 2);
  memcpy(resp + 2, username, strlen(username));
  memcpy(resp + 2 + strlen(username), ";", 1);
  memcpy(resp + 2 + strlen(username) + 1, msg, strlen(msg) + 1);
}

/*
* Methode qui renvoie un message privé pour un autre client sous forme de chaine
* a partir des parametres remplis par le client
*/
void generateWhisp(char *resp, char *username, char *destinataire, char *msg) {
  memcpy(resp, "4;", 2);
  memcpy(resp + 2, username, strlen(username));
  memcpy(resp + 2 + strlen(username), ";", 1);
  memcpy(resp + 2 + strlen(username) + 1, destinataire,
         strlen(destinataire) + 1);
  memcpy(resp + 2 + strlen(username) + 1 + strlen(destinataire), ";", 1);
  memcpy(resp + 2 + strlen(username) + 1 + strlen(destinataire) + 1, msg,
         strlen(msg) + 1);
}

/*
 * Methode qui renvoie un aquittement quand le client a essayé de se connecter
 */
void generateAckLogin(char *resp, int code) {
  memcpy(resp, "5;", 2);
  if (code == 1) {
    memcpy(resp + 2, "1\0", 2);
  } else {
    memcpy(resp + 2, "0\0", 2);
  }
}

/*
* Méthode libérant une structure message_parsed_t
*/
void message_parsed_free(message_parsed_t *message) {
  free(message->text);
  free(message->username);
  free(message->password);
  free(message->destinataire);
}
