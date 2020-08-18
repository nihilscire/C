#include "treePrivate.h"

static node_t* newNode( void );
static node_t* rightRotation(node_t* node);
static node_t* leftRotation(node_t* node);

static int max(int a, int b) { return (a > b) ? a : b; }

static node_t* newNode( void )
{
	node_t *p;

	p = (node_t *)malloc(sizeof(node_t));
	if (p == NULL)
	{
		fprintf(stderr, "Allocation Error.\n");
		exit(1);
	}

	return p;
}

static node_t* rightRotation(node_t* node)
{
	node_t* tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
	return tmp;
}

static node_t* leftRotation(node_t* node)
{
	node_t* tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	return tmp;
}

data_t getData( node_t* node )
{
	return node->val;
}

void setData( node_t* node, data_t data )
{
	node->val = data;
	
	return;
}

node_t* getMinI( node_t* root )
{
	if (root == NULL) return root;

	while (root->left != NULL) 
	{
		root = root->left;
	}

	return root;
}

node_t* getMinR( node_t* root )
{
	if (root == NULL || root->left == NULL) return root;

	return getMinR(root->left);
}

node_t* getMaxI( node_t* root )
{
	if (root == NULL) return root;

	while (root->right != NULL) 
	{
		root = root->right;
	}

	return root;
}

node_t* getMaxR( node_t* root )
{
	if (root == NULL || root->right == NULL) return root;

	return getMaxR(root->right);
}

node_t* searchI( node_t* root, data_t data )
{
	while (root != NULL) 
	{
		if (compare(root->val, data) == 0) return root;

		if (compare(data, root->val) < 0)
			root = root->left;
		else
			root = root->right;
	}

	return NULL;
}

node_t* searchR(node_t* root, data_t data)
{
	if (root == NULL || compare(root->val, data) == 0) 
		return (root);

	if (compare(data, root->val) < 0)
		return (searchR(root->left, data));
	else
		return (searchR(root->right, data));
}

node_t* insert( node_t *root, data_t data )
{
	node_t *p;

	if (root == NULL) 
	{
		p = newNode();
		p->val = data;
		p->left = p->right = NULL;
		return p;
	}

	if (compare(data, root->val) == 0)
	{
		return root;
	}

	if (compare(data, root->val) < 0)
	{
		root->left = insert (root->left, data);
	} 
	else 
	{
		root->right = insert (root->right, data);
	}

	return root;
}

node_t* insertRoot(node_t *root, data_t data)
{
	node_t *p;

	if (root == NULL)
	{
		p = newNode();
		p->val = data;
		p->left = p->right = NULL;
		return p;
	}

	if (compare(data, root->val) == 0)
	{
		return root;
	}

	if (compare(data, root->val) < 0)
	{
		root->left = insert(root->left, data);
		root = rightRotation(root);
	}
	else
	{
		root->right = insert(root->right, data);
		root = leftRotation(root);
	}

	return root;
}

node_t* readFromFile( FILE* fp )
{
	node_t *newTree;
	data_t d;

	newTree = NULL;

	while (readData (fp, &d) != EOF) 
	{
		newTree = insert (newTree, d);
	}

	return newTree;
}

void writeToFile( FILE* fp, node_t* root, int mode)
{
	if (root == NULL) 
	{
		return;
	}

	if (mode == PREORDER) 
	{
		writeData(fp, root->val);
	}

	writeToFile(fp, root->left, mode);

	if (mode == INORDER) 
	{
		writeData(fp, root->val);
	}

	writeToFile(fp, root->right, mode);

	if (mode == POSTORDER) 
	{
		writeData(fp, root->val);
	}

	return;
}

void freeTree( node_t* root )
{
	if (root == NULL) return;

	freeTree(root->left);
	freeTree(root->right);
	free(root);

	return;
}

node_t* removeNode( node_t* root, data_t data )
{
	node_t *p;

	if (root == NULL) 
	{
		//fprintf(stderr, "Error: empty tree\n");
		return root;
	}

	if (compare (data, root->val) < 0)
	{
		root->left = removeNode(root->left, data);
		return root;
	}

	if (compare(data, root->val)> 0)
	{
		root->right = removeNode(root->right, data);
		return root;
	}

	p = root;
	if (root->right == NULL)
	{
		root = root->left;
		free(p);
		return root;
	}

	if (root->left == NULL)
	{
		root = root->right;
		free(p);
		return root;
	}

	p = getMaxR(root);

	root->left = removeNode(root->left, p->val);
	root->val = p->val;

	return root;
}

int height(node_t* root)
{
	if (!root) return 0;

	return max(height(root->left), height(root->right)) + 1;
}

node_t* successor(node_t* root, data_t data)
{
	if (!root) return root;

	if (data < root->val) return successor(root->left, data);

	if (data > root->val) return successor(root->right, data);

	if (root->right != NULL)
	{
		return getMinR(root->right);
	}
	else
	{
		//we need the pointer to the parent
		/*node_t* tmp = root->parent;
		while (tmp && root == tmp->right)
		{
			root = tmp;
			tmp = tmp->parent;
		}
		return tmp;*/
		return NULL;
	}
}

node_t* predecessor(node_t* root, data_t data)
{
	if (!root) return root;

	if (data < root->val) return successor(root->left, data);

	if (data > root->val) return successor(root->right, data);

	if (root->left != NULL)
	{
		return getMaxR(root->left);
	}
	else
	{
		//we need the pointer to the parent
		/*node_t* tmp = root->parent;
		while (tmp && root == tmp->left)
		{
		root = tmp;
		tmp = tmp->parent;
		}
		return tmp;*/
		return NULL;
	}
}

node_t* mirror(node_t* root)
{
	if(root == NULL) return NULL;

	mirror(root->left);
	mirror(root->right);

	node_t* tmp = root->left;
	root->left = root->right;
	root->right = tmp;

	return root;
}

void printTree(node_t* node, int level)
{
    int i;
    if (node != NULL)
    {
        printTree(node->right, level + 1);
        printf("\n\n");

        for (i = 0; i < level; i++) printf("\t");

        printf("%d", node->val);

        printTree(node->left, level + 1);
    }
}