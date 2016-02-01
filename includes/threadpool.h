#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdlib.h>
#include <pthread.h>

typedef struct {
	size_t size;
	int next_job;
	pthread_mutex_t lock;
	pthread_t * pool;
} threadpool_t;

void threadpool_init(threadpool_t * threadpool, size_t size);

void threadpool_run(void * job, void * job_args);

void threadpool_destroy(threadpool_t * threadpool);

#endif
