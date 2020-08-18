#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#define PREORDER  -1
#define INORDER    0
#define POSTORDER  1

#define RED 1
#define BLACK 0

struct node
{
    int key;
    struct node* left;
    struct node* right;
    struct node* parent;
    int color;
};

typedef struct node node_t;

node_t* newNode(int);
node_t* insert(node_t*, int);
void deleteByKey(node_t*, int);

static void insertCase1(node_t*);
static void insertCase2(node_t*);
static void insertCase3(node_t*);
static void insertCase4(node_t*);
static void repairTree(node_t*);
static void deleteCase1(node_t*);
static void deleteCase2(node_t*);
static void deleteCase3(node_t*);
static void deleteCase4(node_t*);
static void deleteCase5(node_t*);
static void deleteCase6(node_t*);

static inline int max(int a, int b) { return (a < b) ? b : a; }

static node_t* getParent(node_t* n) 
{
    if(n == NULL) return NULL;
    return n->parent;
}

static node_t* getGrandParent(node_t* n) 
{
    return getParent(getParent(n));
}

static node_t* getSibling(node_t* n) 
{
    node_t* p = getParent(n);

    if (p == NULL) 
    {
        return NULL;
    }

    if (n == p->left) 
    {
        return p->right;
    } 
    else 
    {
        return p->left;
    }
}

static node_t* getUncle(node_t* n) 
{
    node_t* p = getParent(n);

    return getSibling(p);
}

static void rotateLeft(node_t* root) 
{
    node_t* newRoot = root->right;
    util_check(newRoot != NULL, "leaves cannot become internal nodes"); 
    
    node_t* r = newRoot->left;
    node_t* p = getParent(root);

    root->right = r;
    newRoot->left = root;

    root->parent = newRoot;

    if (r != NULL) 
    {
        r->parent = root;
    }

    if (p != NULL) 
    {
        if (root == p->left) 
        {
            p->left = newRoot;
        } 
        else if (root == p->right) 
        {
            p->right = newRoot;
        }
    }

    newRoot->parent = p;
}

static void rotateRight(node_t* root) 
{
    node_t* newRoot = root->left;
    util_check(newRoot != NULL, "leaves cannot become internal nodes"); 

    node_t* p = getParent(root);
    node_t* l = newRoot->right;

    root->left = l;
    newRoot->right = root;

    root->parent = newRoot;

    if (l != NULL) 
    {
        l->parent = root;
    }

    if (p != NULL) 
    {
        if (root == p->left) 
        {
            p->left = newRoot;
        } 
        else if (root == p->right) 
        {
          p->right = newRoot;
        }
    }

    newRoot->parent = p;
}

node_t* newNode(int key)
{
    node_t* tmp = (node_t*)util_calloc(1, sizeof(node_t));
    tmp->key = key;
    tmp->parent = NULL;
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->color = RED;

    return tmp;
}

static node_t* treeLookup(node_t* root, int key)
{
    if(root == NULL || root->key == key) return root;

    if(key < root->key)
    {
        return treeLookup(root->left, key);
    }
    else
    {
        return treeLookup(root->right, key);
    }

    return root;
}

static node_t* insertRecur(node_t* root, int key, node_t** node)
{
    if (root == NULL)
    {
        *node = newNode(key);
        return *node;
    }

    if(root->key == key)
    {
        *node = root;
        return *node;
    }

    if (key < root->key)
    {
        root->left = insert(root->left, key);
        root->left->parent = root;
    }    
    else if (key > root->key)
    {
        root->right = insert(root->right, key);
        root->right->parent = root;
    }

    *node = root;
    return root;
}

static void insertCase1(node_t* node)
{
    node->color = BLACK;
}

static void insertCase2(node_t* node)
{
    return;
}

static void insertCase3(node_t* node)
{
    getParent(node)->color = BLACK;
    getUncle(node)->color = BLACK;
    getGrandParent(node)->color = RED;
    repairTree(getGrandParent(node));
}

static void insertCase4(node_t* node)
{
    node_t* p = getParent(node);
    node_t* g = getGrandParent(node);

    if (node == p->right && p == g->left) 
    {
        rotateLeft(p);
        node = node->left;
    } 
    else if (node == p->left && p == g->right) 
    {
        rotateRight(p);
        node = node->right;
    }

    if (node == p->left) 
    {
        rotateRight(g);
    } 
    else 
    {
        rotateLeft(g);
    }

    p->color = BLACK;
    g->color = RED;
}

static void repairTree(node_t* node)
{
    if (getParent(node) == NULL) 
    {
        insertCase1(node);
    } 
    else if (getParent(node)->color == BLACK) 
    {
        insertCase2(node);
    } 
    else if (getUncle(node) != NULL && getUncle(node)->color == RED) 
    {
        insertCase3(node);
    } 
    else {
        insertCase4(node);
    }
}

node_t* insert(node_t* root, int key)
{
    node_t* target = NULL;
    root = insertRecur(root, key, &target);

    repairTree(target);

    return root;
}

static void replaceNode(node_t* node, node_t* child) 
{
    child->parent = node->parent;
    
    if (node == node->parent->left) 
    {
        node->parent->left = child;
    } 
    else 
    {
        node->parent->right = child;
    }
}

static void deleteCase1(node_t* node)
{
    if (node->parent != NULL) 
    {
        node_t* s = getSibling(node);

        if (s->color == RED) 
        {
            node->parent->color = RED;
            s->color = BLACK;
            if (node == node->parent->left) 
            {
                rotateLeft(node->parent);
            } 
            else 
            {
                rotateRight(node->parent);
            }
        }

        deleteCase3(node);
    }
}

static void deleteCase2(node_t* node)
{
    node_t* s = getSibling(node);

    if (s->color == RED) 
    {
        node->parent->color = RED;
        s->color = BLACK;
        if (node == node->parent->left) 
        {
            rotateLeft(node->parent);
        } 
        else 
        {
            rotateRight(node->parent);
        }
    }

    deleteCase3(node);
}

static void deleteCase3(node_t* node)
{
    node_t* s = getSibling(node);

    if ((node->parent->color == BLACK) && (s->color == BLACK) &&
      (s->left->color == BLACK) && (s->right->color == BLACK)) 
    {
        s->color = RED;
        deleteCase1(node->parent);
    } 
    else 
    {
        deleteCase4(node);
    }
}

static void deleteCase4(node_t* node)
{
    node_t* s = getSibling(node);

    if ((node->parent->color == RED) && (s->color == BLACK) &&
        (s->left->color == BLACK) && (s->right->color == BLACK)) 
    {
        s->color = RED;
        node->parent->color = BLACK;
    } 
    else 
    {
        deleteCase5(node);
    }
}

static void deleteCase5(node_t* node)
{
    node_t* s = getSibling(node);

    if (s->color == BLACK) 
    {
        if ((node == node->parent->left) && (s->right->color == BLACK) &&
            (s->left->color == RED)) 
        {
            s->color = RED;
            s->left->color = BLACK;
            rotateRight(s);
        } 
        else if ((node == node->parent->right) && (s->left->color == BLACK) &&
                (s->right->color == RED)) 
        {
            s->color = RED;
            s->right->color = BLACK;
            rotateLeft(s);
        }
    }

    deleteCase6(node);
}

static void deleteCase6(node_t* node)
{
    node_t* s = getSibling(node);

    s->color = node->parent->color;
    node->parent->color = BLACK;

    if (node == node->parent->left) 
    {
        s->right->color = BLACK;
        rotateLeft(node->parent);
    } 
    else 
    {
        s->left->color = BLACK;
        rotateRight(node->parent);
    }
}

static void deleteOneChild(node_t* node) 
{
    node_t* child = (node->right == NULL) ? node->left : node->right;
    util_check(child != NULL, "the node must not be a leaf");

    replaceNode(node, child);
    if (node->color == BLACK) 
    {
        if (child->color == RED) 
        {
            child->color = BLACK;
        } 
        else 
        {
            deleteCase1(child);
        }
    }

    free(node);
}

void deleteByKey(node_t* root, int key)
{
    node_t* target = treeLookup(root, key);

    if(target != NULL)
    {
        deleteOneChild(target);
    }
}

int main(int argc, char* argv[])
{
    return 0;
}