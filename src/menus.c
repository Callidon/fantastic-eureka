#include "menus.h"

// Fonction de saisie de texte pour un message
void text_input(WINDOW *win, char * message, size_t max_char) {
	wgetnstr(win, message, max_char);
	// remplacement du retour chariot par un terminateur
	char *p = strchr(message, '\n');
    if(p) {
		*p = '\0';
	}
}

void refresh_window(WINDOW * win) {
	wclear(win);
	wrefresh(win);
}

void print_message(WINDOW * win, char * message) {
	wprintw(win, "%s\n", message);
}

void menu_ask_username(WINDOW * win, char * username) {
	wprintw(win, "Nom d'utilisateur : ");
	wrefresh(win);
	text_input(win, username, MAX_USERNAME_SIZE);
}

void menu_say(WINDOW * win, char * message) {
	wprintw(win, "Message : ");
	wrefresh(win);
	text_input(win, message, MAX_MESSAGE_SIZE);
}

void menu_whisper(WINDOW * win, char * message) {
	char destinataire[MAX_USERNAME_SIZE];
	char text[MAX_MESSAGE_SIZE];

	memset(destinataire, 0, MAX_USERNAME_SIZE);
	memset(text, 0, MAX_MESSAGE_SIZE);

	wprintw(win, "Destinataire : ");
	wrefresh(win);
	text_input(win, destinataire, MAX_USERNAME_SIZE);
	wprintw(win, "Message : ");
	wrefresh(win);
	text_input(win, text, MAX_MESSAGE_SIZE);

	memcpy(message, destinataire, strlen(destinataire));
	memcpy(message + strlen(destinataire), text, strlen(text) + 1);
}
