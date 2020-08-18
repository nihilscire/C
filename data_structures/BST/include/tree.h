#pragma once

#include "data.h"

#define PREORDER  -1
#define INORDER    0
#define POSTORDER  1

typedef struct node node_t;

data_t getData (node_t *);
void setData(node_t *, data_t);

node_t* readFromFile(FILE *);
void writeToFile(FILE *, node_t *, int);
void printTree(node_t*, int);
void freeTree(node_t *);

node_t* searchI (node_t *, data_t);
node_t* searchR (node_t *, data_t);

node_t* getMinI(node_t *);
node_t* getMinR(node_t *);
node_t* getMaxI(node_t *);
node_t* getMaxR(node_t *);

node_t* insert(node_t *, data_t);
node_t* insertRoot(node_t *, data_t);
node_t* removeNode(node_t *, data_t);

node_t* successor(node_t*, data_t);
node_t* predecessor(node_t*, data_t);

node_t* mirror(node_t*);

int height(node_t*);

