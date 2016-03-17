/*
 * Structures et fonctions relatives à la manipulation des fenêtres ncurses
 * Auteurs : Pierre Gaultier & Thomas Minier
 */
#include "menus.h"

/*
 * Fonction effectuant une saisie de texte dans une fenêtre ncurses
 */
void text_input(WINDOW *win, char *message, size_t max_char) {
  char *end_char;
  wgetnstr(win, message, max_char);
  /* remplacement du retour chariot par un terminateur */
  end_char = strchr(message, '\n');
  if (end_char) {
    *end_char = '\0';
  }
}

/*
 * Fonction nettoyant une fenêtre ncurses
 */
void clear_window(WINDOW *win) {
  wclear(win);
  wrefresh(win);
}

/*
 * Fonction affichant un message de multicast dans une fenêtre ncurses
 */
void print_multicast(WINDOW *win, char *message) {
  time_t now = time(0);
  wprintw(win, "%s %s\n", ctime(&now), message);
  wrefresh(win);
}

/*
 * Fonction affichant un message dans une fenêtre ncurses
 */
void print_message(WINDOW *win, char *username, char *message) {
  time_t now = time(0);
  wprintw(win, "%s %s : %s\n", ctime(&now), username, message);
  wrefresh(win);
}

/*
 * Fonction affichant un message privé dans une fenêtre ncurses
 */
void print_whisper(WINDOW *win, char *username, char *message) {
  time_t now = time(0);
  wprintw(win, "%s whisper from %s : %s\n", ctime(&now), username, message);
  wrefresh(win);
}

/*
 * Fonction affichant un accusé d'envoi d'un message privé dans une fenêtre
 * ncurses
 */
void print_ack_whisper(WINDOW *win, char *username, char *message) {
  time_t now = time(0);
  wprintw(win, "%s whisper to %s : %s\n", ctime(&now), username, message);
  wrefresh(win);
}

/*
 * Fonction demandant la saisie d'un nom d'utilisateur dans une fenêtre ncurses
 */
void menu_ask_username(WINDOW *win, char *username) {
  wprintw(win, "Nom d'utilisateur : ");
  wrefresh(win);
  text_input(win, username, MAX_USERNAME_SIZE);
}

/*
 * Fonction demandant la saisie d'un mot de passe dans une fenêtre ncurses
 */
void menu_ask_password(WINDOW *win, char *password) {
  wprintw(win, "Mot de passe : ");
  wrefresh(win);
  text_input(win, password, MAX_PASSWORD_SIZE);
}

/*
 * Fonction demandant la saisie d'un message dans une fenêtre ncurses
 */
void menu_say(WINDOW *win, char *message) {
  wprintw(win, "Message : ");
  wrefresh(win);
  text_input(win, message, MAX_MESSAGE_SIZE);
}

/*
 * Fonction demandant la saisie d'un message privé dans une fenêtre ncurses
 */
void menu_whisper(WINDOW *win, char *destinataire, char *message) {
  wprintw(win, "Destinataire : ");
  wrefresh(win);
  text_input(win, destinataire, MAX_USERNAME_SIZE);
  wprintw(win, "Message : ");
  wrefresh(win);
  text_input(win, message, MAX_MESSAGE_SIZE);
}
