#include "queue.h"

#include <stddef.h>
#include <stdlib.h>

void queue_construct(queue *q)
{
    q->front = nullptr;
    q->back = nullptr;
}

void enqueue(queue *q, void *data)
{
    if (q->back == nullptr) {
        node *n = (node *) malloc(sizeof(node));
        n->data = data;
        n->next = nullptr;

        q->front = n;
        q->back = n;

        return;
    }

    q->back->next = (node *) malloc(sizeof(node));
    q->back->next->data = data;
    q->back->next->next = nullptr;
}

void *dequeue(queue *q)
{
    void *data = q->front->data;
    node *tmp = q->front;
    q->front = q->front->next;
    
    free(tmp);

    return data;
}

void *peek(queue *q)
{
    return q->front->data;
}

void queue_destruct(queue *q)
{
    q->back = nullptr;
    while (q->front != nullptr)
    {
        dequeue(q);
    }
}
