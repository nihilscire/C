#include "util.h"
#include "array.h"

array_t* newArray()
{
    array_t* ret = (array_t*)util_calloc(1, sizeof(array_t));
    ret->data = (int*)util_calloc(ALLOC_SIZE, sizeof(int));
    ret->capacity = ALLOC_SIZE;

    return ret;
}

void deleteArray(array_t** a)
{
    free((*a)->data);
    free(*a);

    *a = NULL;
}

boolean empty(array_t* a)
{
    return (a->size == 0);
}

int get(array_t* a, int pos)
{
    util_check(pos<a->size, "pos must be within limits");

    return a->data[pos];
}

void set(array_t* a, int pos, int data)
{
    util_check(pos<a->size, "pos must be within limits");

    a->data[pos] = data;
}

void append(array_t* a, int data)
{
    if(a->size == a->capacity)
    {
        a->data = (int*)realloc(a->data, (a->capacity+ALLOC_SIZE)*sizeof(int));
        a->capacity += ALLOC_SIZE;
    }

    a->data[a->size] = data;
    a->size++;
}

void erase(array_t* a, int pos)
{
    util_check(pos<a->size, "pos must be within limits");

    unsigned int i;

    for(i = pos; i < a->size-1; ++i)
    {
        a->data[i] = a->data[i+1];
    }

    a->data[a->size-1] = 0;
    a->size--;
}

void print(array_t* a, FILE* out)
{
    unsigned int i;

    for(i = 0; i < a->size; ++i)
    {
        fprintf(out, "%d ", a->data[i]);
    }

    fprintf(out, "\n");
}

int size(array_t* a)
{
    return a->size;
}

void resize(array_t* a, unsigned int n)
{
    int* tmp = a->data;
    if(n > a->capacity)
    {
        tmp = (int*)util_calloc(n, sizeof(int));
        memcpy(tmp, a->data, a->size*sizeof(int));
        a->data = tmp;
    }

    if(n < size)
    {
        unsigned int i;
        for(i = n; i < a->capacity; ++i)
        {
            a->data[i] = 0;
        }
    }

    a->size = n;
}