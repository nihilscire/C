#pragma once

#include <stdio.h>

struct node
{
    void* data;
    size_t size;
    struct node* next;
    struct node* prev;
};

struct list
{
    struct node* head;
    struct node* tail;
};

struct list_iterator
{
    struct list* list;
    struct node* curr;
};

typedef struct node node_t;
typedef struct list list_t;
typedef struct list_iterator list_iterator_t;

list_t* newList(void);
void deleteList(list_t**);
void insertHead(list_t*, void*, size_t);
void insertTail(list_t*, void*, size_t);
void eraseHead(list_t*);
void eraseTail(list_t*);
void* getFront(list_t*);
void* getBack(list_t*);
int empty(list_t*);
int length(list_t*);
void print(list_t*, FILE*, void (*printData)(void*, FILE*));
list_iterator_t* begin(list_t*);
list_iterator_t* end(list_t*);
int isValidIt(list_iterator_t*);
void nextIt(list_iterator_t*);
void* derefIt(list_iterator_t*);
void deleteIt(list_iterator_t*);