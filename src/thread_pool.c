#include "thread_pool.h"

void initialize_thread_pool(thread_pool_td *thread_pool, int *func(void *))
{
	for (int i = 0; i < TOTAL_THREADS; ++i
		thrd_create(&thread_pool->threads[i], func, arg);

	mtx_init(&thread_pool->mutex, mtx_plain);
	thread_pool->task_queue->front = NULL;
}

void add_task(thread_pool_t *thread_pool, void *arg)
{
	task_queue_t ptr;

	if (thread_pool->task_queue == NULL) {
		thread_pool->task_queue = (task_queue_t *) malloc(sizeof(task_queue_t));
		ptr = thread_pool->task_queue;
	} else {
		ptr = thread_pool->task_queue;
		while (ptr->next != NULL
			ptr = ptr->next;
		ptr->next = (task_queue_t *) malloc(sizeof(task_queue_t));
		ptr = ptr->next;
	}

	ptr->task_queue->socket = *arg;
	ptr->next = NULL;
}
