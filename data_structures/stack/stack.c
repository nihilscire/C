
#include "list.h"
#include "util.h"

struct stack
{
    list_t* list;
    int size;
};

typedef struct stack stack_t;

stack_t* newStack(void);
void deleteStack(stack_t**);

void push(stack_t*, int);
int pop(stack_t*);

int isEmpty(stack_t*);

int size(stack_t*);

stack_t* newStack(void)
{
    stack_t* s = (stack_t*)util_calloc(1, sizeof(stack_t));

    s->list = newList();

    return s;
}

void deleteStack(stack_t** s)
{
    deleteList(&(*s)->list);

    free(*s);

    *s = NULL;
}

void push(stack_t* s, int data)
{
    insertTail(s->list, data);
    s->size++;
}

int pop(stack_t* s)
{
    util_check(!empty(s->list), "the stack is empty");

    int ret = eraseTail(s->list);
    s->size--;

    return ret;
}

int isEmpty(stack_t* s)
{
    return (s->size == 0);
}

int size(stack_t* s)
{
    return s->size;
}

int main(int argc, char* argv[])
{
    stack_t* stack = newStack();

    unsigned int i;

    for(i = 0; i < 10; ++i)
    {
        push(stack, i);
    }

    while(!isEmpty(stack))
    {
        fprintf(stdout, "%d ", pop(stack));
    }

    return 0;
}