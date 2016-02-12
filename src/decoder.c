#include "decoder.h"

/*
* Methode qui renvoie une structure a partir d'un message afin de le traiter dans le serveur
*/
message_parsed_t * decode(char * msg){

	char * s1;
	char * s2;
	char * s3;
	char * token;
	token = strtok(msg,";");

	message_parsed_t *msg_prd = malloc(sizeof(message_parsed_t));

	switch(atoi(token)){

		case 0 :
		msg_prd->code = 0;
		token = strtok(NULL,";");
		msg_prd->text = strdup(token);
		msg_prd->username = NULL;
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 1 :
		msg_prd->code = 1;
		token = strtok(NULL,";");
		msg_prd->text = NULL;
		msg_prd->username = strdup(token);
		token = strtok(NULL,";");
		msg_prd->password = strdup(token);
		msg_prd->destinataire = NULL;
		break;

		case 2 :
		msg_prd->code = 2;
		token = strtok(NULL,";");
		msg_prd->text = NULL;
		msg_prd->username = strdup(token);
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 3 :
		msg_prd->code = 3;
		token = strtok(NULL,";");
		msg_prd->text = strdup(token);
		token = strtok(NULL,";");
		msg_prd->username = strdup(token);
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 4 :
		msg_prd->code = 4;
		token = strtok(NULL,";");
		msg_prd->text = strdup(token);
		token = strtok(NULL,";");
		msg_prd->username = strdup(token);
		msg_prd->password = NULL;
		token = strtok(NULL,";");
		msg_prd->destinataire = strdup(token);
		break;
	}

	return  msg_prd;
}

/*
 * Concatène deux strings
 */
void stringcats(char **str, const char *str2) {
    char *tmp = NULL;

    // Reset *str
    if ( *str != NULL && str2 == NULL ) {
        free(*str);
        *str = NULL;
        return;
    }

    // Initial copy
    if (*str == NULL) {
        *str = calloc( strlen(str2)+1, sizeof(char) );
        memcpy( *str, str2, strlen(str2) );
    }
    else { // Append
        tmp = calloc( strlen(*str)+1, sizeof(char) );
        memcpy( tmp, *str, strlen(*str) );
        *str = calloc( strlen(*str)+strlen(str2)+1, sizeof(char) );
        memcpy( *str, tmp, strlen(tmp) );
        memcpy( *str + strlen(*str), str2, strlen(str2) );
        free(tmp);
    }
}

/*
* Methode qui renvoie un message multicast sous forme de chaine a partir des parametres remplis par le client
*/
char* generateMulticast(char* msg){
	char* res = malloc(strlen(msg) + 2);
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
char* generateMsg(char* username , char* msg){
	char* res = malloc(strlen(username) + 3 + strlen(msg));
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
