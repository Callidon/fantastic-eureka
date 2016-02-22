#ifndef MENUS_H
#define MENUS_H

#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "config.h"

void text_input(WINDOW *win, char * message, size_t max_char);

void refresh_window(WINDOW * win);

void print_message(WINDOW * win, char * message);

void menu_ask_username(WINDOW * win, char * username);

void menu_say(WINDOW * win, char * message);

void menu_whisper(WINDOW * win, char * message);

#endif
