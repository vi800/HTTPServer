#ifndef THREADS_H
#define THREADS_H

#include <threads.h>

#define TOTAL_THREADS 16

typedef struct task {
	socket_t socket;
	struct task *next;
} task_queue_t;

typedef struct {
	thrd_t threads[TOTAL_THREADS];
	task_queue_t task_queue;
} thread_pool_t;

#endif
