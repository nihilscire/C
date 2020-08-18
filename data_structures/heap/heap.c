#include "array.h"
#include "util.h"

struct heap
{
    array_t* array;
};

typedef struct heap heap_t;

heap_t* newHeap(void);
void deleteHeap(heap_t**);

int isEmpty(heap_t*);
int getSize(heap_t*);

heap_t* buildMaxHeap(int*, size_t);

void push(heap_t*, int);
int pop(heap_t*);
int max(heap_t*);

static void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void maxHeapify(heap_t* h, int i)
{
    int l = 2*i;
    int r = 2*i+1;
    int largest = i;

    int* a = h->array->data;

    if(l < getSize(h) && a[l] > a[largest])
    {
        largest = l;
    }

    if(r < getSize(h) && a[r] > a[largest])
    {
        largest = r;
    }

    if(largest != i)
    {
        swap(&a[i], &a[largest]);
        maxHeapify(h, largest);
    }
}

heap_t* newHeap()
{
    heap_t* h = (heap_t*)util_calloc(1, sizeof(heap_t));

    h->array = newArray();

    return h;
}

void deleteHeap(heap_t** h)
{
    deleteArray(&(*h)->array);

    free(*h);

    *h = NULL;
}

int isEmpty(heap_t* h)
{
    return empty(h->array);
}

int getSize(heap_t* h)
{
    return size(h->array);
}

heap_t* buildMaxHeap(int* a, size_t size)
{
    heap_t* h = newHeap();

    int i;

    for(i = 0; i < size; ++i)
    {
        append(h->array, a[i]);
    }

    for(i = size/2; i >= 0; --i)
    {
        maxHeapify(h, i);
    }

    return h;
}

static void heapifyUp(heap_t* h, int i)
{
    int p = (i-1) / 2;

    if(p < 0) return;

    int* a = h->array->data;

    if(a[i] > a[p])
    {
        swap(&a[i], &a[p]);
        heapifyUp(h, p);
    }
}

void push(heap_t* h, int data)
{
    append(h->array, data);
    heapifyUp(h, getSize(h)-1);
}

int pop(heap_t* h)
{
    util_check(!isEmpty(h), "the heap is empty!");

    int ret = max(h);

    erase(h->array, 0);

    maxHeapify(h, 0);

    return ret;
}

int max(heap_t* h)
{
    return get(h->array, 0);
}

int main(int argc, char* argv[])
{
    int a[] = {4,1,3,2,16,9,10,14,8,7};
    int size = sizeof(a)/sizeof(a[0]);

    heap_t* heap = buildMaxHeap(a, size);

    unsigned int i;
    for(i = 0; i < getSize(heap); ++i)
    {
        fprintf(stdout, "%d ", heap->array->data[i]);
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "max: %d\n", max(heap));

    pop(heap);

    fprintf(stdout, "max: %d\n", max(heap));

    pop(heap);
    
    fprintf(stdout, "max: %d\n", max(heap));

    return 0;
}