#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>

// Tests pour le rendu avec un thread qui print et le main qui attend des inputs

// Version naïve
void * yolo_render(void * nothing) {
	int i;
	for(i = 0; i < 30; i++) {
		printf("Choupette\n");
		sleep(5);
	}
}

void yolo_version() {
	char message[256];
	pthread_t thread;

	pthread_create(&thread, NULL, yolo_render, NULL);

	for(;;) {
		fgets(message, 256, stdin);
	}
}

// version avec ncurses
void * ncurses_render(void * window) {
	int i;
	for(i = 1; i < 30; i++) {
		mvwprintw(window, i, 1, "Choupette");
		wrefresh(window);
		sleep(5);
	}
}

void ncurses_version() {
	WINDOW *up, *down;
	pthread_t thread;

	initscr();

	up = subwin(stdscr, LINES/2, COLS, 0, 0);
	down = subwin(stdscr, LINES/2, COLS, LINES/2, 0);

	box(up, ACS_VLINE, ACS_HLINE);
	box(down, ACS_VLINE, ACS_HLINE);

	//mvwprintw(up, 1, 1, "Fentre du haut");
	pthread_create(&thread, NULL, ncurses_render, (void *) up);
	mvwprintw(down, 1, 1, "Fenêtre du bas");

	//wrefresh(up);
	wrefresh(down);

	getch();
	endwin();

	delwin(up);
	delwin(down);
}

// main
int main(void) {
	//yolo_version();
	//ncurses_version();
	return 0;
}
