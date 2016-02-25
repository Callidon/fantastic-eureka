/*
 * Structures et fonctions relatives à la manipulation des fenêtres ncurses
 * Auteurs : Pierre Gaultier & Thomas Minier
 */
#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "config.h"

/*
 * Fonction effectuant une saisie de texte dans une fenêtre ncurses
 */
void text_input(WINDOW * win, char * message, size_t max_char);

/*
 * Fonction nettoyant une fenêtre ncurses
 */
void clear_window(WINDOW * win);

/*
 * Fonction affichant un message de multicast dans une fenêtre ncurses
 */
void print_multicast(WINDOW * win, char * message);

/*
 * Fonction affichant un message dans une fenêtre ncurses
 */
void print_message(WINDOW * win, char * username, char * message);

/*
 * Fonction affichant un message privé dans une fenêtre ncurses
 */
void print_whisper(WINDOW * win, char * username, char * message);

/*
 * Fonction demandant la saisie d'un nom d'utilisateur dans une fenêtre ncurses
 */
void menu_ask_username(WINDOW * win, char * username);

/*
 * Fonction demandant la saisie d'un message dans une fenêtre ncurses
 */
void menu_say(WINDOW * win, char * message);

/*
 * Fonction demandant la saisie d'un message privé dans une fenêtre ncurses
 */
void menu_whisper(WINDOW * win, char * destinataire, char * message);

#endif
