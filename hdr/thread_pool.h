#ifndef THREADS_H
#define THREADS_H

#include "queue.h"

#include <threads.h>

#define TOTAL_THREADS 16

typedef struct {
	thrd_t pool[TOTAL_THREADS];
    bool active;
	queue work;
	mtx_t lock;
    cnd_t signal;
} thread_pool_t;

typedef struct {
    void (*job)(void *arg);
    void *arg;
} thread_job_t;

void initialize_thread_pool(thread_pool_t *);
int thread_function(void *);
void thread_pool_add_job(thread_pool_t *, void (*job)(void *), void *arg);

#endif
