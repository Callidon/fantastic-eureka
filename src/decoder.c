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
		msg_prd->code = Multicast;
		token = strtok(NULL,";");
		msg_prd->text = strdup(token);
		msg_prd->username = NULL;
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 1 :
		msg_prd->code = Login;
		token = strtok(NULL,";");
		msg_prd->text = NULL;
		msg_prd->username = strdup(token);
		token = strtok(NULL,";");
		msg_prd->password = strdup(token);
		msg_prd->destinataire = NULL;
		break;

		case 2 :
		msg_prd->code = Leave;
		token = strtok(NULL,";");
		msg_prd->text = NULL;
		msg_prd->username = strdup(token);
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 3 :
		msg_prd->code = Say;
		token = strtok(NULL,";");
		msg_prd->text = strdup(token);
		token = strtok(NULL,";");
		msg_prd->username = strdup(token);
		msg_prd->password = NULL;
		msg_prd->destinataire = NULL;
		break;

		case 4 :
		msg_prd->code = Whisper;
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
void generateMulticast(char* resp, char* msg){
	memcpy(resp,"0;",2);
  memcpy(resp+2,msg,strlen(msg)+1);
}

/*
* Methode qui renvoie un message de login sous forme de chaine a partir des parametres remplis par le client
*/
void generateLogin(char* resp,char* username, char* password){
	memcpy(resp,"1;",2);
	memcpy(resp+2,username,strlen(username));
	memcpy(resp+2+strlen(username),";",1);
	memcpy(resp+2+strlen(username)+1,password,strlen(password)+1);
}

/*
* Methode qui renvoie un message de deconnexionsous forme de chaine a partir des parametres remplis par le client
*/
void generateLeave(char* resp,char* username){
	memcpy(resp,"2;",2);
	memcpy(resp+2,username,strlen(username)+1);
}

/*
* Methode qui renvoie un message pour tout les autres clients sous forme de chaine a partir des parametres remplis par le client
*/
void generateMsg(char* resp,char* username , char* msg){
	memcpy(resp,"3;",2);
	memcpy(resp+2,username,strlen(username));
	memcpy(resp+2+strlen(username),";",1);
	memcpy(resp+2+strlen(username)+1,msg,strlen(msg)+1);
}


/*
* Methode qui renvoie un message privé pour un autre client sous forme de chaine a partir des parametres remplis par le client
*/
void generateWhisp(char* resp,char* username, char* destinataire, char* msg){
	memcpy(resp,"3;",2);
	memcpy(resp+2, username, strlen(username));
	memcpy(resp+2 + strlen(username), ";", 1);
	memcpy(resp+2 + strlen(username) + 1,destinataire,strlen(destinataire) + 1);
	memcpy(resp+2 + strlen(username) + 1 + strlen(destinataire), ";",1);
	memcpy(resp+2 + strlen(username) + 1 + strlen(destinataire) + 1, msg, strlen(msg) + 1);
	
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
