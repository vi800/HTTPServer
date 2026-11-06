#ifndef QUEUE_H
#define QUEUE_H

typedef struct node {
    void *data;
    struct node *next;
} node;

typedef struct {
    node *front;
    node *back;
} queue;

void queue_construct(queue *);
void enqueue(queue *, void *);
void *dequeue(queue *);
void *peek(queue *);
void queue_destruct(queue *);

#endif
