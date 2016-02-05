#include "decoder.h"

/*
 * Methode qui renvoie une structure a partir d'un message afin de le traiter dans le serveur
 */
 message_parsed_t * decode(char * msg){

  char* code = strtok(msg,";");
  char* s1;
  char* s2;
  char* s3;

  message_parsed_t *msg_prd = malloc(sizeof(message_parsed_t));

  switch(*code){

    case 0 :
      s1 = strtok(msg,";");
      msg_prd->code = 0;
	  printf("strtok : %s", s1);
      msg_prd->text = strdup(s1);
      msg_prd->username = NULL;
      msg_prd->password = NULL;
      msg_prd->destinataire = NULL;
      break;

    case 1 :
      s1 = strtok(msg,";");
      s2 = strtok(msg,";");
      msg_prd->code = 1;
      msg_prd->text = NULL;
      msg_prd->username = strdup(s1);
      msg_prd->password = strdup(s2);
      msg_prd->destinataire = NULL;
      break;

    case 2 :
      s1 = strtok(msg,";");
      msg_prd->code = 2;
      msg_prd->text = NULL;
      msg_prd->username = strdup(s1);
      msg_prd->password = NULL;
      msg_prd->destinataire = NULL;
      break;

    case 3 :
      s1 = strtok(msg,";");
      s2 = strtok(msg,";");
      msg_prd->code = 3;
      msg_prd->text = strdup(s2);
      msg_prd->username = strdup(s1);
      msg_prd->password = NULL;
      msg_prd->destinataire = NULL;
      break;

    case 4 :
      s1 = strtok(msg,";");
      s2 = strtok(msg,";");
      s3 = strtok(msg,";");
      msg_prd->code = 4;
      msg_prd->text = strdup(s3);
      msg_prd->username = strdup(s1);
      msg_prd->password = NULL;
      msg_prd->destinataire = strdup(s2);
      break;
  }

  return  msg_prd;

}
