#ifdef _MSC_VER
#include "stdafx.h"
#endif // _MSC_VER

#include "tree.h"

int main(int argc, char* argv[])
{
	node_t *root, *tmp;
	data_t d = 4;
	FILE *fp = NULL;

	root = NULL;

	fp = fopen("input.txt", "r");
	if (fp == NULL) 
	{
		fprintf(stderr, "Error opening file\n");
		return 1;
	}
	
	root = readFromFile(fp);
	fclose(fp);

	writeToFile(stdout, root, POSTORDER);

	printTree(root, 0);

	/*
	tmp = searchR(root, d);
	writeData(stdout, getData(tmp));

	d = 3;
	root = removeNode(root, d);

	fp = fopen("output.txt", "w");
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return 1;
	}

	writeToFile(fp, root, POSTORDER);
	fclose(fp);
	*/

	freeTree(root);

	return 0;
}

