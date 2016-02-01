#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdlib.h>
#include <pthread.h>

typedef struct {
	pthread_t thread;
	pthread_mutex_t lock;
} worker_t;

typedef struct {
	size_t size;
	int next_job;
	worker_t ** pool;
	pthread_mutex_t lock;
} threadpool_t;

void threadpool_init(threadpool_t * threadpool, size_t size);

void threadpool_run(threadpool_t * threadpool, void * job, void * job_args);

void threadpool_destroy(threadpool_t * threadpool);

#endif
