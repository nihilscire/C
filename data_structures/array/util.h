#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void util_check(unsigned int expr, char* msg);

FILE* util_fopen(char* name, char* mode);

void* util_malloc(unsigned int size);

void* util_calloc(unsigned int num, size_t size);

char* util_strdup(char* src);

void util_array_dispose(void** ptr, unsigned int n, void (*quit)(void *));

void** util_matrix_alloc(unsigned int n, unsigned int m, unsigned int size);

void util_matrix_dispose(void*** ptr, unsigned int n, unsigned int m, 
                         void (*quit)(void *));