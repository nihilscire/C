#include "util.h"

void util_check(unsigned int expr, char* msg)
{
    if ( !(expr) ) 
    {
        fprintf(stderr, "Error: %s\n", msg);
        exit(EXIT_FAILURE);
    }
}

FILE* util_fopen(const char* name, const char* mode)
{
    FILE* fp = fopen(name, mode);
    util_check(fp!=NULL, "could not open file!");
    return fp;
}

void* util_malloc(unsigned int size)
{
    void* ptr = malloc(size);
    util_check(ptr!=NULL, "memory allocation failed!");
    return ptr;
}

void* util_calloc(unsigned int num, size_t size)
{
    void* ptr = calloc(num, size);
    util_check(ptr!=NULL, "memory allocation failed!");
    return ptr;
}

char* util_strdup(char* src)
{
    char* dst = strdup(src);
    util_check(dst!=NULL, "memory allocation failed");
    return dst;
}

void util_array_dispose(void** ptr, unsigned int n, void (*quit)(void *))
{
    unsigned int i;

    if (quit != NULL) 
    {
        for (i = 0; i < n; i++) 
        {
            quit(ptr[i]);
        }
    }
    free(ptr);
}

void** util_matrix_alloc(unsigned int n, unsigned int m, unsigned int size)
{
    void** ptr;
    unsigned int i;

    ptr = (void **)util_malloc(n * sizeof(void *));
    for (i = 0; i < n; i++) 
    {
        ptr[i] = util_calloc(m, size);
    }
    return ptr;
}

void util_matrix_dispose(void*** ptr, unsigned int n, unsigned int m, 
                         void (*quit)(void *))
{
    unsigned int i, j;

    for (i = 0; i < n; i++) 
    {
        if (quit != NULL) 
        {
            for (j = 0; j < m; j++) 
            {
                quit(ptr[i][j]);
            }
        }
        free(ptr[i]);
    }
    free(ptr);
}

void die(int line, const char* format, ...)
{
    va_list vargs;
    va_start (vargs, format);
    fprintf (stderr, "%d: ", line);
    vfprintf (stderr, format, vargs);
    fprintf (stderr, "\n");
    va_end (vargs);

    err(EXIT_FAILURE, NULL);
}

size_t util_getline(char* buff, size_t size, FILE* stream)
{
    fgets(buff, size, stream);

    char* p = strchr(buff, '\n');

    if(p) *p = '\0';

    return strlen(buff);
}