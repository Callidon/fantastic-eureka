#include "decoder.h"

/*
* Methode qui renvoie une structure a partir d'un message afin de le traiter dans le serveur
*/
message_parsed_t * decode(char * msg){

	char * s1;
	char * s2;
	char * s3;
	msg = strtok(msg,";");
	message_parsed_t *msg_prd = malloc(sizeof(message_parsed_t));

	switch(*msg){

		case 0 :
		msg_prd->code = 0;
		msg = strtok(NULL,";");
		printf("strtok : %s", msg);
		msg_prd->text = strdup(msg);
		msg_prd->username = NULL;
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 1 :
		msg_prd->code = 1;
		msg = strtok(NULL,";");
		msg_prd->text = NULL;
		msg_prd->username = strdup(msg);
		msg = strtok(NULL,";");
		msg_prd->password = strdup(msg);
		msg_prd->destinataire = NULL;
		break;

		case 2 :
		msg_prd->code = 2;
		msg = strtok(NULL,";");
		msg_prd->text = NULL;
		msg_prd->username = strdup(msg);
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 3 :
		msg_prd->code = 3;
		msg = strtok(NULL,";");
		msg_prd->text = strdup(msg);
		msg = strtok(NULL,";");
		msg_prd->username = strdup(msg);
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 4 :
		msg_prd->code = 4;
		msg = strtok(NULL,";");
		msg_prd->text = strdup(msg);
		msg = strtok(NULL,";");
		msg_prd->username = strdup(msg);
		msg_prd->password = NULL;
		msg = strtok(NULL,";");
		msg_prd->destinataire = strdup(msg);
		break;
	}

	return  msg_prd;
}

/*
* Methode qui renvoie un message multicast sous forme de chaine a partir des parametres remplis par le client
*/
char* generateMulticast(char* msg){
	char* res = malloc(strlen(msg)+2);
	strcpy(res, "0;");
	strcat(res, msg);
	return res;
}

/*
* Methode qui renvoie un message de login sous forme de chaine a partir des parametres remplis par le client
*/
char* generateLogin(char* username, char* password){
	char* res = malloc(strlen(username)+3+strlen(password));
	strcpy(res, "1;");
	strcat(res, username);
	strcat(res, ";");
	strcat(res, password);
	return res;
}

/*
* Methode qui renvoie un message de deconnexionsous forme de chaine a partir des parametres remplis par le client
*/
char* generateLeave(char* username){
	char* res = malloc(strlen(username)+2);
	strcpy(res, "2;");
	strcat(res, username);
	return res;
}

/*
* Methode qui renvoie un message pour tout les autres clients sous forme de chaine a partir des parametres remplis par le client
*/
char* generateMsg(char* username ,char* msg){
	char* res = malloc(strlen(username)+3+strlen(msg));
	strcpy(res, "3;");
	strcat(res, username);
	strcat(res, ";");
	strcat(res, msg);
	return res;
}


/*
* Methode qui renvoie un message privé pour un autre client sous forme de chaine a partir des parametres remplis par le client
*/
char* generateWhisp(char* username, char* destinataire, char* msg){
	char* res = malloc(strlen(username)+4+strlen(destinataire)+strlen(msg));
	strcpy(res, "3;");
	strcat(res, username);
	strcat(res, ";");
	strcat(res, destinataire);
	strcat(res, ";");
	strcat(res, msg);
	return res;
}

/*
 * Méthode libérant une structure message_parsed_t
 */
void message_parsed_free(message_parsed_t * message) {
	free(message->text);
	free(message->username);
	free(message->password);
	free(message->destinataire);
}
