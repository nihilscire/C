#pragma once

#include <stdio.h>
#include <stdlib.h>

#define ALLOC_SIZE 5

struct array
{
    int* data;
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

int size(array_t*);

void resize(array_t*, unsigned int);