#include "array.h"
#include "util.h"

#define BUCKETS 50

struct hashTable
{
    array_t* array;
    int M;
};

typedef struct hashTable ht_t;

ht_t* newHashTable(int);
void deleteHashTable(ht_t**);

int getByKey(ht_t*, char*);
void setByKey(ht_t*, char*, int);

static int hash(char* s, int M)
{
    int h = 0, base = 127;

    for ( ; *s != '\0'; ++s)
        h = (base * h + *s) % M;

    return h;
}

static void resizeHT(){}

ht_t* newHashTable(int M)
{
    ht_t* ht = (ht_t*)util_calloc(1, sizeof(ht_t));

    ht->array = newArray();
    ht->M = M;

    resize(ht->array, M);

    return ht;
}

void deleteHashTable(ht_t** ht)
{
    deleteArray(&(*ht)->array);

    free(*ht);

    *ht = NULL;
}

int getByKey(ht_t* ht, char* k)
{
    int h = hash(k, ht->M);

    return get(ht->array, h);
}

void setByKey(ht_t* ht, char* k, int v)
{
    int h = hash(k, ht->M);
    set(ht->array, h, v);
}

int main(int argc, char* argv[])
{
    ht_t* ht = newHashTable(BUCKETS);

    char key[] = "hello";

    setByKey(ht, key, 42);

    fprintf(stdout, "value: %d\n", getByKey(ht, key));

    return 0;
}