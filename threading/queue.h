#pragma once

#include <string.h>
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

void enqueue(queue_t*, void*, size_t);
void* dequeue(queue_t*);

int isEmpty(queue_t*);
int size(queue_t*);