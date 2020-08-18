#include <string.h>
#include "util.h"
#include "list.h"

static node_t* newNode(void*, size_t);
static void deleteNode(node_t*);
static void deleteRecur(node_t*);
static list_iterator_t* newIterator(void);

static node_t* newNode(void* data, size_t size)
{
    node_t* n = (node_t*)util_calloc(1, sizeof(node_t));

    n->data = util_calloc(1, size);

    memcpy(n->data, data, size);

    return n;
}

static void deleteNode(node_t* n)
{
    free(n->data);
    free(n);
}

static void deleteRecur(node_t* n)
{
    if(n == NULL) return;

    deleteRecur(n->next);

    deleteNode(n);
}

static list_iterator_t* newIterator()
{
    list_iterator_t* it = (list_iterator_t*) util_calloc(1, sizeof(list_iterator_t));

    return it;
}

list_t* newList(void)
{
    list_t* l = (list_t*)util_calloc(1, sizeof(list_t));

    return l;
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

void insertHead(list_t* l, void* data, size_t size)
{
    node_t* n = newNode(data, size);
    
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

void insertTail(list_t* l, void* data, size_t size)
{
    node_t* n = newNode(data, size);
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

void* getFront(list_t* l)
{
    util_check(!empty(l), "the list is empty!");

    return l->head->data;
}

void* getBack(list_t* l)
{
    util_check(!empty(l), "the list is empty!");

    return l->tail->data;
}

void print(list_t* l, FILE* fp, void (*printData)(void*, FILE*))
{
    node_t* tmp = l->head;

    while(tmp)
    {
        //fprintf(fp, "%d ", tmp->data);
        printData(tmp->data, fp);
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

list_iterator_t* begin(list_t* l)
{
    list_iterator_t* it = newIterator();
    it->list = l;
    it->curr = l->head;

    return it;
}

// nope don't use it like it != end(list)
// use isValidIt(it) instead
list_iterator_t* end(list_t* l)
{
    return NULL;
}

int isValidIt(list_iterator_t* it)
{
    return (it->curr != NULL);
}

void nextIt(list_iterator_t* it)
{
    util_check(it->curr != NULL, "the iterator points to NULL!");

    it->curr = it->curr->next;
}

void* derefIt(list_iterator_t* it)
{
    util_check(it->curr != NULL, "the iterator points to NULL!");

    void* ret = it->curr->data;

    return ret;
}

void deleteIt(list_iterator_t* it)
{
    free(it);
}