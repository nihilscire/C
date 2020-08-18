#include <stdio.h>
#include <stdlib.h>

#define PREORDER  -1
#define INORDER    0
#define POSTORDER  1

struct node
{
    int key;
    struct node *left;
    struct node *right;
    int height;
};
typedef struct node AVLNode;

static inline int max(int a, int b) { return (a < b) ? b : a; }

AVLNode *newNode(int key)
{
    AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));

    if (newNode == NULL)
    {
        fprintf(stderr, "Allocation error!\n");
        exit(-1);
    }

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 0;

    return newNode;
}

int getHeight(AVLNode *node)
{
    if (node == NULL) return -1;
        
    return (node->height);
}

int heightDiff(AVLNode *node)
{
    if (node == NULL) return 0;
    
    return (getHeight(node->left) - getHeight(node->right));
}

AVLNode *getMin(AVLNode *root)
{
    if (root == NULL || root->left == NULL) return root;

	return getMin(root->left);
}

AVLNode *rightRotation(AVLNode *node)
{
    AVLNode *l = node->left;
    AVLNode *r = l->right;

    node->left = r;
    l->right = node;
    
    node->height = (max(getHeight(node->left), getHeight(node->right)) + 1);
    l->height = (max(getHeight(l->left), getHeight(l->right)) + 1);

    return l;
}

AVLNode *leftRotation(AVLNode *node)
{
    AVLNode *r = node->right;
    AVLNode *l = r->left;

    node->right = l;
    r->left = node;

    node->height = (max(getHeight(node->left), getHeight(node->right)) + 1);
    r->height = (max(getHeight(r->left), getHeight(r->right)) + 1);

    return r;
}

AVLNode *leftRightRotation(AVLNode *node)
{
    node->left = leftRotation(node->left);

    return (rightRotation(node));
}

AVLNode *rightLeftRotation(AVLNode *node)
{
    node->right = rightRotation(node->right);

    return (leftRotation(node));
}

AVLNode *insert(AVLNode *node, int key)
{
    if (node == NULL) return newNode(key);

    if(node->key == key) return node;

    if (key < node->key)
    {
        node->left = insert(node->left, key);
    }    
    else if (key > node->key)
    {
        node->right = insert(node->right, key);
    }

    node->height = (max(getHeight(node->left), getHeight(node->right)) + 1);

    int balance = heightDiff(node);

    if (balance > 1 && key < (node->left->key))
        return rightRotation(node);

    if (balance > 1 && key > (node->left->key))
    {
        node = leftRightRotation(node);
    }

    if (balance < -1 && key > (node->right->key))
        return leftRotation(node);

    if (balance < -1 && key < (node->right->key))
    {
        node = rightLeftRotation(node);
    }

    return node;
}

AVLNode *delete (AVLNode *node, int key)
{
    AVLNode *tmp = NULL;

    if (node == NULL) return node;

    if (key < node->key)
    {
        node->left = delete (node->left, key);
    }
    else if (key > node->key)
    {
        node->right = delete (node->right, key);
    }
    else
    {
        if ((node->left == NULL) || (node->right == NULL))
        {
            tmp = node->left ? node->left : node->right;

            if (tmp == NULL)
            {
                tmp = node;
                node = NULL;
            }
            else 
            {
                *node = *tmp;
            }

            free(tmp);
        }
        else
        {
            tmp = getMin(node->right);
            node->key = tmp->key;
            node->right = delete (node->right, tmp->key);
        }
    }

    if (node == NULL) return node;

    node->height = (max(getHeight(node->left), getHeight(node->right)) + 1);

    int balance = heightDiff(node);

    if ((balance > 1) && (heightDiff(node->left) >= 0))
    {
        return rightRotation(node);
    }

    if ((balance > 1) && (heightDiff(node->left) < 0))
    {
        node = leftRightRotation(node);
    }

    if ((balance < -1) && (heightDiff(node->right) >= 0))
    { 
        return leftRotation(node);
    }

    if ((balance < -1) && (heightDiff(node->right) < 0))
    {
        node = rightLeftRotation(node);
    }

    return node;
}

AVLNode* search(AVLNode *root, int key)
{
    if (root == NULL || root->key ==  key) 
		return root;

	if (key < root->key)
		return search(root->left, key);
	else
		return search(root->right, key);
}

void printTree(AVLNode *node, int level)
{
    int i;
    if (node != NULL)
    {
        printTree(node->right, level + 1);
        printf("\n\n");

        for (i = 0; i < level; i++) printf("\t");

        printf("%d", node->key);

        printTree(node->left, level + 1);
    }
}

void printOrder( FILE* fp, AVLNode* root, int mode)
{
	if (root == NULL) 
	{
		return;
	}

	if (mode == PREORDER) 
	{
		fprintf(fp, "%d\n", root->key);
	}

	printOrder(fp, root->left, mode);

	if (mode == INORDER) 
	{
		fprintf(fp, "%d\n", root->key);
	}

	printOrder(fp, root->right, mode);

	if (mode == POSTORDER) 
	{
		fprintf(fp, "%d\n", root->key);
	}

	return;
}

int main(int argc, char* argv[])
{

    return 0;
}
