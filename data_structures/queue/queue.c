
#include "list.h"
#include "util.h"

struct queue
{
    list_t* list;
    int size;
};

typedef struct queue queue_t;

queue_t* newQueue(void);
void deleteQueue(queue_t**);

void enqueue(queue_t*, int);
int dequeue(queue_t*);

int isEmpty(queue_t*);

int size(queue_t*);

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

void enqueue(queue_t* q, int data)
{
    insertTail(q->list, data);
    q->size++;
}

int dequeue(queue_t* q)
{
    util_check(!empty(q->list), "the Queue is empty");

    int ret = eraseHead(q->list);
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

int main(int argc, char* argv[])
{
    queue_t* queue = newQueue();

    unsigned int i;

    for(i = 0; i < 10; ++i)
    {
        enqueue(queue, i);
    }

    while(!isEmpty(queue))
    {
        fprintf(stdout, "%d ", dequeue(queue));
    }

    return 0;
}