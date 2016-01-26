// Programme de test pour montrer l'utilisation des pthreads
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define NB_THREADS 5

typedef struct {
	int cpt;
	pthread_t threads_add [NB_THREADS];
} storage_t;

static storage_t storage = {
	.cpt = 0,
};

static void * fn_add(void * p_data) {
	int nb = (intptr_t) p_data;
	printf("Thread %d : adding %d to the datas\n", nb, nb);
	storage.cpt += 1;
}

int main(int argc, char **argv) {
	int i = 0;
	int ret = 0;
	// on spawn des threads
	for(i = 0; i < NB_THREADS; i++) {
		ret = pthread_create(&storage.threads_add [i], NULL, fn_add, (void *) (intptr_t) i);

		if(ret) {
			fprintf(stderr, "%d", strerror(ret));
		}
	}

	// on attend la fin de l'exécution de chaque thread
	i = 0;
	for(i = 0; i < NB_THREADS; i++) {
		pthread_join(storage.threads_add[i], NULL);
	}
	printf("Tous les threads ont fini de tourner. Le résultat final est %d\n", storage.cpt);
	return 0;
}
