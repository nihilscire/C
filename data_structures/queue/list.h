#pragma once

#include <stdio.h>

struct node
{
    int data;
    struct node* next;
    struct node* prev;
};

struct list
{
    struct node* head;
    struct node* tail;
};

typedef struct node node_t;
typedef struct list list_t;

list_t* newList(void);
void deleteList(list_t**);

void insertHead(list_t*, int);
void insertTail(list_t*, int);

int eraseHead(list_t*);
int eraseTail(list_t*);

int empty(list_t*);
int length(list_t*);

void print(list_t*, FILE*);