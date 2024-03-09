/*
	I implemented two algorithms for transformations between representations of the same tree (R1, R2, R3) and displayed them using the Pretty-Print format.

	For T1, from the parent representation to the multi-child representation: I designed an iterative function in which I add children for each element of the tree, traversing 
	the parent vector provided as input only once. Thus, for each new child found, I reallocate memory, place it in the corresponding position in the element's children vector, 
	and increment the number of children. The complexity of the function is O(n), where n is the number of nodes.

	For T2, from the multi-child representation to the binary representation: I designed a recursive function that uses a for loop to traverse the children vector of each 
	element, and along the way, determines if the current node is the child of the parent or the sibling of the child. The complexity of the function is O(n), where n is 
	the number of nodes.

	For Pretty-Print display: initially, I thought of an iterative function (commented-out code) that used a search function in the parent vector and sequentially 
	searched for each child of the root, displaying spaces and the node in accordance with the element's position. However, in the end, I opted for implementing a recursive 
	function, which was much easier and shorter to implement.
*/

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

typedef struct parent {
	int key;
	int parinte;
}parent;

typedef struct multi {
	int key;
	multi **children;
	int nr_ch;
}multi;

typedef struct binary {
	int key;
	binary *children;
	binary *sibling;
}binary;

/*
int cauta(int* v, int n, int root, int nr)
{
	for (int i = 0; i < n; i++)
		if(v[i]== root)
		{
			v[i] = 0;		//change the value from the vector
			return i + 1;   
		}
	return n + 1;	//return for a value not found
}

void parinte_print(int* w, int n, int root)
{
	printf("%d", root+1);

	int* roots = (int*)calloc(n, sizeof(int));
	int k = 0;
	roots[0] = root+1;

	int nr = 2;
	while(roots[0]!=0)
	{
		int r = cauta(w, n, roots[k], nr);	//search for a child
	
		if (r != n + 1)		//if not found
		{
			printf("\n");
			for (int j = 0; j < nr; j++) //print the no. of spaces and the root
				printf(" ");
			printf("%d", r);
			nr+=2;
			roots[++k] = r;		//a new parent to search for its children
		}
		else               //the element is not root
		{
			nr-=2;
			roots[k] = 0;	//eliminate from vector so i don't come back for it's parent
			k--;
		}	
	}
}
*/

void pretty_print(int* v, int n, int root, int lvl)
{
	for (int i = 0; i < lvl; i++)
		printf(" ");

	printf("%d\n", root+1);

	for (int i = 0; i < n; i++)
		if (v[i] == root+1)
			pretty_print(v, n, i, lvl + 2);

}

void R1(parent* p, int* v, int n, int &root)
{
	for (int i = 0; i < n; i++)
	{
		p[i].key = i + 1;
		p[i].parinte = v[i];
		if (v[i] == -1)
			root = i + 1;
	}
}

multi* T1(int* v, int n)
{
	multi* r = (multi*)calloc(n, sizeof(multi));

	for (int i = 0; i < n; i++)
	{
		r[i].key = i+1;
		r[i].nr_ch = 0;
		r[i].children = NULL;
	}

	int root = 0;
	for (int i = 0; i < n; i++)
		if (v[i] != -1)
		{
			r[v[i]-1].children = (multi**)realloc(r[v[i]-1].children, (r[v[i]-1].nr_ch + 1) * sizeof(multi*));
			r[v[i]-1].children[r[v[i]-1].nr_ch] = &r[i];
			r[v[i] - 1].nr_ch++;
		}
		else
			root = i;

	return &r[root];
}

void pretty_print_T1(multi* root, int lvl)
{
	for (int i = 0; i < lvl; i++)
		printf(" ");

	if (root != NULL) 
	{
		printf("%d\n", root->key);
		for (int i = 0; i < root->nr_ch; i++)
		{
			pretty_print_T1(root->children[i], lvl+2);
		}
	}
}

binary* T2( multi* root, int n)
{
	if (root != NULL)
	{
		binary* binaryRoot = (binary*)calloc(n, sizeof(binary));	
		binaryRoot->key = root->key;	
		binaryRoot->children = NULL;
		binaryRoot->sibling = NULL;

		binary* binaryChild = NULL;
		for (int i = 0; i < root->nr_ch; i++)
		{
			binary* binaryTree= T2(root->children[i], n);

			if (binaryTree != NULL)
			{
				if (binaryChild == NULL)
					binaryRoot->children = binaryTree;
				else
					binaryChild->sibling = binaryTree;
				binaryChild = binaryTree;
			}
		}
		return binaryRoot;
	}
}

void printBinaryTree(binary* root, int level) 
{
	if (root != NULL) 
	{
		for (int i = 0; i < level; i++) 
			printf("  ");
	
		printf("%d\n", root->key);
		printBinaryTree(root->children, level + 1);
		printBinaryTree(root->sibling, level);
	}
}

void demo()
{
	int n = 9, root = 0;
	int v[] = { 2,7,5,2,7,7,-1,5,2 };
	/*int* w = (int*)calloc(n, sizeof(int));
	for (int i = 0; i < n; i++)
	{ 
		w[i] = v[i];
		if (w[i] == -1)
			root = i;
	}
	//printf("Reprezentarea arborelui parinte (R1):\n");
	//parinte_print(w, n, root);*/

	////Pretty Print R1
	printf("Reprezentarea R1:\n");
	for (int i = 0; i < n; i++)
		if (v[i] == -1)
			root = i;
	pretty_print(v, n, root, 0);

	////Pretty Print T1
	printf("\nTransformare R1 -> R2:\n");
	multi* r = T1(v, n);
	pretty_print_T1(r, 0);
	
	////Pretty Print T2
	printf("\nTransformare R2 -> R3:\n");
	binary* b = T2(r, n);
	printBinaryTree(b, 0);
}

int main()
{
	demo();
	return 0;
}