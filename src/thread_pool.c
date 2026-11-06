#include "thread_pool.h"
#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

void initialize_thread_pool(thread_pool_t *thread_pool)
{
    thread_pool->active = true;
    queue_construct(&thread_pool->work);
    mtx_init(&thread_pool->lock, mtx_plain);
    cnd_init(&thread_pool->signal);
    for (int i = 0; i < TOTAL_THREADS; ++i)
    {
        thrd_create(&thread_pool->pool[i], &thread_function, thread_pool);
    }
}

int thread_function(void *arg)
{
    thread_pool_t *thread_pool = (thread_pool_t *) arg;
    thread_job_t *thread_job;
    while (thread_pool->active)
    {
        mtx_lock(&thread_pool->lock);
        if (thread_pool->work.front != nullptr) 
        {
            thread_job = (thread_job_t *) dequeue(&thread_pool->work);
            
            printf("func: %p\n", thread_job->job);
            printf("arg: %p\n", thread_job->arg);
            thread_job->job(thread_job->arg);
            free(thread_job->arg);    
        }
        mtx_unlock(&thread_pool->lock);
    }

    return 0;
}

void thread_pool_add_job(thread_pool_t *thread_pool, void (*job)(void *), void *arg)
{
    thread_job_t *thread_job = (thread_job_t *) malloc(sizeof(thread_job_t));
    thread_job->job = job;
    thread_job->arg = arg;
    
    mtx_lock(&thread_pool->lock);
    enqueue(&thread_pool->work, thread_job);
    mtx_unlock(&thread_pool->lock);
}
