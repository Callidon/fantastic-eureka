/*
 * Client à lancer pour se connecter à un serveur hébergeant un salon de
 * discussion
 * usage : ./client <adresse-serveur>
 * Auteurs : Pierre Gaultier & Thomas Minier
 */
#include "config.h"
#include "handler.h"
#include "menus.h"
#include <linux/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

int main(int argc, char **argv) {

  pthread_t thread_handler;     /* Thread du handler côté client */
  render_datas_t *render_datas; /* données à passer au thread de rendu */
  sockaddr_in adresse_locale;   /* adresse de socket local */
  hostent *ptr_host;            /* info sur une machine hote */
  int socket_descriptor,        /* descripteur de socket */
      fileDescriptors[2], /* descripteur pour le pipe entre le main process & le
                             thread
                             d'écoute */
      successful_login = 0; /* Booléen indiquant si l'authentifgication s'est
                               faite avec succès */
  char buffer[MAX_BUFFER_SIZE], /* buffer de réception des messages venant du
                                   serveur */
      message[MAX_BUFFER_SIZE], /* message envoyé */
      username[MAX_USERNAME_SIZE], /* nom d'utilisateur */
      password[MAX_PASSWORD_SIZE], /* Mot de passe */
      destinataire
          [MAX_USERNAME_SIZE], /* Nom du destinataire d'un message privé */
      *host;                   /* nom de la machine distante */
  WINDOW *wchat, /* Pointeur vers la fenêtre d'affichage du chat */
      *winput;   /* Pointeur vers la fenêtre d'es inputs utilisateurs */

  memset(buffer, 0, MAX_BUFFER_SIZE);
  memset(message, 0, MAX_BUFFER_SIZE);
  memset(username, 0, MAX_USERNAME_SIZE);

  /* vérification et récupération des arguments */
  if (argc != 2) {
    perror("usage : client <adresse-serveur>");
    exit(1);
  }

  host = argv[1];

  if (pipe(fileDescriptors) < 0) {
    perror("Erreur : impossible de créer un pipe entre le main process et le "
           "thread d'écoute");
    exit(1);
  };

  if ((ptr_host = gethostbyname(host)) == NULL) {
    perror(
        "erreur : impossible de trouver le serveur a partir de son adresse.");
    exit(1);
  }

  /* copie caractere par caractere des infos de ptr_host vers adresse_locale */
  bcopy((char *)ptr_host->h_addr, (char *)&adresse_locale.sin_addr,
        ptr_host->h_length);
  adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype; */

  adresse_locale.sin_port = htons(5001);

  printf("numero de port pour la connexion au serveur : %d \n",
         ntohs(adresse_locale.sin_port));

  /* creation de la socket */
  if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(
        "erreur : impossible de creer la socket de connexion avec le serveur.");
    exit(1);
  }

  /* tentative de connexion au serveur dont les infos sont dans adresse_locale
   */
  if ((connect(socket_descriptor, (sockaddr *)(&adresse_locale),
               sizeof(adresse_locale))) < 0) {
    perror("erreur : impossible de se connecter au serveur.");
    exit(1);
  }

  /* passage en mode ncurses */
  initscr();

  wchat = subwin(stdscr, LINES / 2, COLS, 0, 0);
  winput = subwin(stdscr, LINES / 2, COLS, LINES / 2, 0);
  scrollok(wchat, TRUE);
  scrollok(winput, TRUE);

  wrefresh(wchat);
  wrefresh(winput);

  /* stockage des données et lancement du thread pour le rendu du chat */
  render_datas = malloc(sizeof(render_datas_t));
  render_datas->window = wchat;
  render_datas->socket = socket_descriptor;
  render_datas->fileDescriptor = fileDescriptors[1];
  if (pthread_create(&thread_handler, NULL, client_handler, render_datas) < 0) {
    perror("erreur : impossible de créer un nouveau thread");
    exit(1);
  }

  /* demande de l'username & du password tant que l'on n'est pas connecté */
  while (!successful_login) {

    menu_ask_username(winput, username);
    wrefresh(wchat);
    menu_ask_password(winput, password);
    wrefresh(wchat);

    /* transmission des saisies au serveur pour validation */
    generateLogin(message, username, password);
    write(socket_descriptor, message, strlen(message) + 1);
    clear_window(winput);

    read(fileDescriptors[0], buffer, sizeof(buffer));

    if (atoi(buffer) == 1) {
      successful_login = 1;
    } else {
      wprintw(winput, "Erreur : mot de passe incorrect. Veuillez réessayer\n");
    }
  }

  wprintw(wchat, "Connecté en tant que %s\n", username);
  wrefresh(wchat);

  /* sélection d'action */
  for (;;) {
    memset(buffer, 0, MAX_BUFFER_SIZE);
    wprintw(winput, "Tapez le numéro correspondant à l'action désirée :\n");
    wprintw(winput, "1 : Envoyer un message\n2 : Envoyer un message privé\n3 : "
                    "Déconnexion\n");
    wprintw(winput, "Action : ");
    wrefresh(winput);

    /* en fonction du numéro de l'action saisie */
    switch (wgetch(winput)) {
    /* cas d'un message à envoyer */
    case 49: {
      wprintw(winput, "\n");
      menu_say(winput, buffer);
      wrefresh(winput);

      /* envoi du message au serveur */
      generateMsg(message, username, buffer);
      write(socket_descriptor, message, strlen(message) + 1);
    } break;
    /* cas d'un whisper à envoyer */
    case 50: {
      wprintw(winput, "\n");
      menu_whisper(winput, destinataire, buffer);
      wrefresh(winput);

      /* envoi du message au serveur */
      generateWhisp(message, username, destinataire, buffer);
      write(socket_descriptor, message, strlen(message) + 1);

      /* affichage de l'accusé d'envoi */
      print_ack_whisper(wchat, destinataire, buffer);
      wrefresh(wchat);
    } break;
    /* cas d'une déconnexion à effectuer */
    case 51: {
      generateLeave(message, username);
      write(socket_descriptor, message, strlen(message) + 1);

      /* on attend la fin du thread du handler avant de fermer le programme */
      pthread_join(thread_handler, NULL);

      /* fin du programme */
      endwin();
      delwin(wchat);
      delwin(winput);
      exit(0);
    } break;
    default: {
      wprintw(winput, "\n");
      menu_say(winput, buffer);
      wprintw(winput, "Action inconnue\n");
    }
    }
    clear_window(winput);
  }
}
