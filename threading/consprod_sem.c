#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#include "util.h"
#include "queue.h"

#define CONS 5
#define PROD 5

typedef void* (*thread_fun_t)(void*);

struct thread
{
    pthread_t th;
    int id;
    thread_fun_t f;
};

typedef struct thread thread_t;

queue_t* production;

int arr[5];
int idx;

pthread_mutex_t mutex;

sem_t emptySem;
sem_t fullSem;

void* consumer(void* arg)
{
    int id = *((int*)arg);

    int* tmp = NULL;

    srand(id);

    while(1)
    {
        sem_wait(&fullSem);

        pthread_mutex_lock(&mutex);

        tmp = (int*)dequeue(production);
        //tmp = arr[idx--];
        //printf("consumer %d, idx = %d, tmp = %d\n", id, idx, tmp);
        //idx--;

        fprintf(stdout, "I am thread consumer %d: %d\n", id, *tmp);

        pthread_mutex_unlock(&mutex);

        sem_post(&emptySem);

        free(tmp);
        sleep(rand() % 2 + 1);
    }

    return NULL;
}

void* producer(void* arg)
{
    int id = *((int*)arg);

    int tmp = 0;

    srand(id);

    while(1)
    {
        tmp = rand() % 100 + 1;

        sem_wait(&emptySem);

        pthread_mutex_lock(&mutex);

        fprintf(stdout, "I am thread producer %d: %d\n", id, tmp);
        
        enqueue(production, &tmp, sizeof(tmp));
        //arr[++idx] = tmp;
        //printf("producer %d, idx = %d, tmp = %d\n", id, idx, tmp);

        pthread_mutex_unlock(&mutex);

        sem_post(&fullSem);

        sleep(rand() % 2 + 1); 
    }


    return NULL;
}

int main()
{
    int i;
    int ret;
    int id = 0;

    production = newQueue();

    idx = -1;
    for(i = 0; i < 5; ++i) arr[i] = -1;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&fullSem, 0, 0);
    sem_init(&emptySem, 0, 5);

    thread_t* consThreads = (thread_t*) util_calloc(CONS, sizeof(thread_t));
    thread_t* prodThreads = (thread_t*) util_calloc(PROD, sizeof(thread_t));

    for(i = 0; i < CONS; ++i)
    {
        consThreads[i].id = id;
        id++;
        consThreads[i].f = consumer;
    }

    for(i = 0; i < PROD; ++i)
    {
        prodThreads[i].id = id;
        id++;
        prodThreads[i].f = producer;
    }

    for(i = 0; i < CONS; ++i)
    {
        thread_t* tmp = &consThreads[i];
        ret = pthread_create(&tmp->th, NULL, tmp->f, &tmp->id);
        if(ret != 0) die(__LINE__, "pthread_create failed with thread id %d\n", tmp->id);
    }

    for(i = 0; i < PROD; ++i)
    {
        thread_t* tmp = &prodThreads[i];
        ret = pthread_create(&tmp->th, NULL, tmp->f, &tmp->id);
        if(ret != 0) die(__LINE__, "pthread_create failed with thread id %d\n", tmp->id);
    }

    for(i = 0; i < CONS; ++i)
    {
        pthread_join(consThreads[i].th, NULL);
    }

    for(i = 0; i < PROD; ++i)
    {
        pthread_join(prodThreads[i].th, NULL);        
    }

    free(consThreads);
    free(prodThreads);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&fullSem);
    sem_destroy(&emptySem);

    deleteQueue(&production);

    return 0;
}