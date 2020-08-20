#include "queue.h"

queue_t* newQueue(void)
{
    queue_t* q = (queue_t*)util_calloc(1, sizeof(queue_t));

    q->list = newList();

    return q;
}

void deleteQueue(queue_t** q)
{
    deleteList(&(*q)->list);

    free(*q);

    *q = NULL;
}

void enqueue(queue_t* q, void* data, size_t size)
{
    insertTail(q->list, data, size);
    q->size++;
}

void* dequeue(queue_t* q)
{
    util_check(!empty(q->list), "the queue is empty");

    void* ret = getFront(q->list);

    eraseHead(q->list);
    q->size--;

    return ret;
}

int isEmpty(queue_t* q)
{
    return (q->size == 0);
}

int size(queue_t* q)
{
    return q->size;
}