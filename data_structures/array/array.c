#include <stdio.h>
#include <stdlib.h>
#include "util.h"

#define ALLOC_SIZE 5

struct array
{
    int* arr;
    size_t size;
    size_t capacity;
};

typedef struct array array_t;
typedef unsigned char boolean;

array_t* newArray(void);
void deleteArray(array_t**);

boolean empty(array_t*);

int get(array_t*, int);
void set(array_t*, int, int);

void append(array_t*, int);

void erase(array_t*, int);

void print(array_t*, FILE*);

array_t* newArray()
{
    array_t* ret = (array_t*)util_calloc(1, sizeof(array_t));
    ret->arr = (int*)util_calloc(ALLOC_SIZE, sizeof(int));
    ret->capacity = ALLOC_SIZE;

    return ret;
}

void deleteArray(array_t** a)
{
    free((*a)->arr);
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

    return a->arr[pos];
}

void set(array_t* a, int pos, int data)
{
    util_check(pos<a->size, "pos must be within limits");

    a->arr[pos] = data;
}

void append(array_t* a, int data)
{
    if(a->size == a->capacity)
    {
        a->arr = (int*)realloc(a->arr, (a->capacity+ALLOC_SIZE)*sizeof(int));
        a->capacity += ALLOC_SIZE;
    }

    a->arr[a->size] = data;
    a->size++;
}

void erase(array_t* a, int pos)
{
    util_check(pos<a->size, "pos must be within limits");

    unsigned int i;

    for(i = pos; i < a->size-1; ++i)
    {
        a->arr[i] = a->arr[i+1];
    }

    a->arr[a->size-1] = 0;
    a->size--;
}

void print(array_t* a, FILE* out)
{
    unsigned int i;

    for(i = 0; i < a->size; ++i)
    {
        fprintf(out, "%d ", a->arr[i]);
    }

    fprintf(out, "\n");
}

int main(int argc, char* argv[])
{
    array_t* array = NULL;

    array = newArray();

    int i;

    for(i = 0; i < 10; ++i)
    {
        append(array, i);
    }

    print(array, stdout);

    erase(array, 2);

    print(array, stdout);

    deleteArray(&array);

    return 0;
}