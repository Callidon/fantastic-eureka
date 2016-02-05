#include "decoder.h"


/*
 * Methode qui renvoie une structure a partir d'un message afin de le traiter dans le serveur
 */
 message_parsed *decode(char* msg){

  char* code = strtok(msg,";");
  char* s1;
  char* s2;
  char* s3;
	
  message_parsed *msg_prd = malloc(sizeof( message_parsed));

  switch(*code){

    case 0 :
      s1 = strtok(msg,";");
      msg_prd->code = "0";
      msg_prd->message = strdup(s1);
      msg_prd->username = NULL;
      msg_prd->password = NULL;
      msg_prd->destinataire = NULL;
      break;

    case 1 :
      s1 = strtok(msg,";");
      s2 = strtok(msg,";");
      msg_prd->code = "1";
      msg_prd->message = NULL;
      msg_prd->username = strdup(s1);
      msg_prd->password = strdup(s2);
      msg_prd->destinataire = NULL;
      break;

    case 2 :
      s1 = strtok(msg,";");
      msg_prd->code = "2";
      msg_prd->message = NULL;
      msg_prd->username = strdup(s1);
      msg_prd->password = NULL;
      msg_prd->destinataire = NULL;
      break;

    case 3 :
      s1 = strtok(msg,";");
      s2 = strtok(msg,";");
      msg_prd->code = "3";
      msg_prd->message = strdup(s2);
      msg_prd->username = strdup(s1);
      msg_prd->password = NULL;
      msg_prd->destinataire = NULL;
      break;

    case 4 :
      s1 = strtok(msg,";");
      s2 = strtok(msg,";");
      s3 = strtok(msg,";");
      msg_prd->code = "4";
      msg_prd->message = strdup(s3);
      msg_prd->username = strdup(s1);
      msg_prd->password = NULL;
      msg_prd->destinataire = strdup(s2);
      break;
  }
    
  return  msg_prd;

}
