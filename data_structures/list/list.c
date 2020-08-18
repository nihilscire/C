#include "util.h"
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

void eraseHead(list_t*);
void eraseTail(list_t*);

int empty(list_t*);
int length(list_t*);

void print(list_t*, FILE*);

static node_t* newNode(int data)
{
    node_t* n = (node_t*)util_calloc(1, sizeof(node_t));

    n->data = data;

    return n;
}

static void deleteNode(node_t* n)
{
    free(n);
}

list_t* newList(void)
{
    list_t* l = (list_t*)util_calloc(1, sizeof(list_t));

    return l;
}

static void deleteRecur(node_t* n)
{
    if(n == NULL) return;

    deleteRecur(n->next);

    deleteNode(n);
}

void deleteList(list_t** l)
{
    if(!empty(*l))
    {
        deleteRecur((*l)->head);
    } 

    free(*l);

    *l = NULL;
}

void insertHead(list_t* l, int data)
{
    node_t* n = newNode(data);
    
    node_t* tmp = l->head;
    if(empty(l)) 
    {
        l->tail = n;
    }

    if(!empty(l))
    {
        tmp->prev = n;
    }
    
    l->head = n;
    n->next = tmp;

}

void insertTail(list_t* l, int data)
{
    node_t* n = newNode(data);
    node_t* tmp = l->tail;

    if(empty(l)) 
    {
        l->head = n;
    }

    if(!empty(l))
    {
        tmp->next = n;
    }

    l->tail = n;
    n->prev = tmp;
}

void eraseHead(list_t* l)
{
    if(empty(l)) return;

    node_t* tmp = l->head;
    l->head = tmp->next;
    deleteNode(tmp);

    if(empty(l))
    {
        l->tail = NULL;
    }
}

void eraseTail(list_t* l)
{
    if(empty(l)) return;    

    node_t* tmp = l->tail;
    l->tail = tmp->prev;
    deleteNode(tmp);

    if(empty(l))
    {
        l->head = NULL;
    }
}

void print(list_t* l, FILE* fp)
{
    node_t* tmp = l->head;

    while(tmp)
    {
        fprintf(fp, "%d ", tmp->data);
        tmp = tmp->next;
    }

    fprintf(fp, "\n");    
}

int empty(list_t* l)
{
    return (l->head == NULL) || (l->tail == NULL);
}

int length(list_t* l)
{
    if(empty(l)) return 0;

    node_t* tmp = l->head;
    int count = 0;

    while(tmp)
    {
        count++;
        tmp = tmp->next;
    }

    return count;
}

int main(int argc, char* argv[])
{
    list_t* list = NULL;
    list = newList();

    unsigned int i;

    for(i = 0; i < 10; ++i)
    {
        insertHead(list, i);
    }

    print(list, stdout);

    return 0;
}