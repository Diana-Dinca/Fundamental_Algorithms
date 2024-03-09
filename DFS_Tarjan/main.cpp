/*
	I have implemented the Depth-First Search (DFS) algorithm, topological sorting, and Tarjan's algorithm for strongly connected components using adjacency lists. 
	To demonstrate the variation in execution time of the DFS algorithm, I created two graphs. Firstly, I fixed the number of nodes to 100 and varied the number of edges 
	between 1000 and 4500. Secondly, I fixed the number of edges at 4500 and varied the number of nodes between 100 and 200. The generated graphs show a uniform, 
	linear increase, as the algorithm has a complexity of O(V + E).

*/

#include<stdlib.h>
#include<stdio.h>

#include "Profiler.h"
Profiler pro("Lab11");

enum { WHITE = 0, GRAY, BLACK };

typedef struct node {
	int key;

	int color;
	int start;
	int finish;
	node*  parent;

	int aSize;
	node** a;

	//Tarjan
	int index;
	int comp;
	bool onStack;
	int lowLink;
}node;

typedef struct graph {
	int n;
	int m;
	node** v;
}graph;

typedef struct nodeList {
	node* nod;
	nodeList* next;
}nodeList;

typedef struct List {
	nodeList* first;
	nodeList* last;
	int size;
}List;

List* createList()
{
	List* l = (List*)malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return l;
}

void insert_first(List* l, node* nod)
{
	nodeList* p = (nodeList*)malloc(sizeof(nodeList));
	p->next = NULL;
	p->nod = nod;
	if (l->first == NULL) //lista e goala
	{
		l->first = p;
		l->last = p;
	}
	else //exista elemente in lista
	{
		p->next = l->first;
		l->first = p;
	}
	l->size++;
}

int t;
void DFS_VISIT(graph* g, node* u, List* list, int* ciclu, Operation op)
{
	t++;
	u->start = t;
	u->color = GRAY;
	op.count(2);
	for (int i = 0; i < u->aSize; i++)
	{
		op.count();
		if (u->a[i]->color == WHITE)
		{
			op.count();
			u->a[i]->parent = u;
			DFS_VISIT(g, u->a[i], list, ciclu, op);
		}
		//op.count();
		if (u->a[i]->color == GRAY)
			*ciclu = 1;
	}
	op.count(2);
	u->color = BLACK;
	t++;
	u->finish = t;
	insert_first(list, u);
}

void DFS(graph* g, List* list, int* ciclu, Operation op)
{
	for (int u = 0; u < g->n; u++)
	{
		op.count(2);
		g->v[u]->color = WHITE;
		g->v[u]->parent = NULL;
	}
	for (int u = 0; u < g->n; u++)
	{
		op.count();
		if (g->v[u]->color == WHITE)
			DFS_VISIT(g, g->v[u], list, ciclu, op);
	}
}

void afisareGraf(graph g)
{
	printf("Arborele initial este: \n");
	for (int i = 0; i < g.n; i++)
	{
		printf("%d: ", g.v[i]->key);
		for (int j = 0; j < g.v[i]->aSize; j++)
		{
			printf("%d", g.v[i]->a[j]->key);
			if (j < g.v[i]->aSize - 1)
				printf(", ");
		}

		printf("\n");
	}
}

void afisareArbore(graph g)
{
	int* aux = (int*)malloc(g.n*sizeof(int));
	int k = 0;
	for (int i = 1; i < g.n*2; i++)
	{
		for (int j = 0; j < g.n; j++)
		{
			if (g.v[j]->start == i)
			{
				aux[k++]= g.v[j]->key;
				break;
			}
		}
	}
	printf("Muchiile arborelui DFS sunt: ");
	for (int i = 0; i < k; i++)
	{
		//printf("%d ", aux[i]);
		if (g.v[aux[i]-1]->parent != NULL)
			printf("(%d, %d) ", g.v[aux[i]-1]->parent->key, g.v[aux[i]-1]->key);
	}
	printf("\n");
}

node* delete_first(List* l)
{
	node* p = l->first->nod;
	nodeList* q = l->first;
	l->first = q->next; //urmatorul nod devine capul listei
	free(q); //eliberam capul listei
	return p;
}

List* S;
int index, nrComp;
int mini(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

void strong_connect(graph* g, node* u)
{
	u->index = index;
	u->lowLink = index;
	index++;
	insert_first(S, u);
	u->onStack = true;
	
	for (int i = 0; i < u->aSize; i++)
		if (u->a[i]->index == -1)
		{
			strong_connect(g, u->a[i]);
			u->lowLink = mini(u->lowLink, u->a[i]->lowLink);
		}
		else if (u->a[i]->onStack== true)
			u->lowLink = mini(u->lowLink, u->a[i]->index);
	node* p;
	if (u->lowLink == u->index)
	{
		nrComp++;
		printf("SCC%d: ",nrComp);
		do {
		    p = delete_first(S);
			p->onStack = false;
			p->comp = nrComp;
			printf("%d ", p->key);
		} while (p!= u);
		printf("\n");
	}
}

void Tarjan(graph* g)
{
	for (int i = 0; i < g->n; i++)
	{
		g->v[i]->index = -1;
		g->v[i]->lowLink = -1;
		g->v[i]->onStack = false;
		g->v[i]->comp = 0;
	}
	for (int i = 0; i < g->n; i++)
		if (g->v[i]->index == -1)
			strong_connect(g, g->v[i]);
}


void demo()
{
	graph g;
	g.n = 6;
	g.m = 8;
	g.v = (node**)calloc(g.n, sizeof(node*));
	for (int i = 0; i < g.n; i++)
	{
		g.v[i] = (node*)calloc(g.n, sizeof(node));
		g.v[i]->key = i + 1;
	}
	//graf1
	/*
	g.v[0]->aSize = 2;
	g.v[1]->aSize = 1;
	g.v[2]->aSize = 1;
	g.v[3]->aSize = 0;
	g.v[4]->aSize = 2;
	g.v[5]->aSize = 1;
	*/

	//graf2
	///*
	g.v[0]->aSize = 2;
	g.v[1]->aSize = 1;
	g.v[2]->aSize = 1;
	g.v[3]->aSize = 0;
	g.v[4]->aSize = 2;
	g.v[5]->aSize = 0;
	//*/

	for (int i = 0; i < g.n; i++)
		g.v[i]->a = (node**)calloc(g.v[i]->aSize, sizeof(node*));
	//graf1
	/*
	g.v[0]->a[0] = g.v[2]; //1
	g.v[0]->a[1] = g.v[4];
	g.v[1]->a[0] = g.v[5]; //2
	g.v[2]->a[0] = g.v[3]; //3
	g.v[4]->a[0] = g.v[1]; //5
	g.v[4]->a[1] = g.v[2];
	g.v[5]->a[0] = g.v[4]; //6
	*/

	//graf2
	///*
    g.v[0]->a[0] = g.v[2]; //1
	g.v[0]->a[1] = g.v[4];
	g.v[1]->a[0] = g.v[5]; //2
	g.v[2]->a[0] = g.v[3]; //3
	g.v[4]->a[0] = g.v[1]; //5
	g.v[4]->a[1] = g.v[2];
	//g.v[5]->a[0] = g.v[4]; //6
	//*/

	afisareGraf(g);
	printf("\n");

	List* list = createList();
	int ciclu = 0;
	Operation op = pro.createOperation("DFS- muchii", g.m);
	DFS(&g, list, &ciclu, op);
	afisareArbore(g);
	printf("\n");

	//afisareGraf(g);
	
	if (ciclu == 1)
		printf("Graful prezinta ciclu! Nu se poate face sortare Topologica\n");
	else
	{
		printf("Graful e aciclic!");
		printf("Sortarea Topologica este: ");
		nodeList* p = list->first;
		while (p != NULL)
		{
			printf("%d ", p->nod->key);
			p = p->next;
		}
		printf("\n");
	}
	printf("\n");

	S = createList();
	printf("Componentele puternic conexe ale grafului sunt: \n");
	Tarjan(&g);
}

void free_graph(graph* g)
{
	if (g->v != NULL) {
		for (int i = 0; i < g->n; ++i) {
			if (g->v[i] != NULL) {
				if (g->v[i]->a != NULL) {
					free(g->v[i]->a);
					g->v[i]->a = NULL;
				}
				g->v[i]->aSize = 0;
				free(g->v[i]);
				g->v[i] = NULL;
			}
		}
		free(g->v);
		g->v = NULL;
	}
	g->n = 0;
}

void perf()
{
	//V= 100 
	for (int e = 1000; e <= 4500; e += 100)
	{
		graph g;
		g.n = 100;
		g.m = e;

		g.v = (node**)calloc(g.n, sizeof(node*));
		for (int i = 0; i < g.n; i++)
		{
			g.v[i] = (node*)calloc(g.n, sizeof(node));
			g.v[i]->key = i + 1;
			g.v[i]->a = (node**)malloc(g.m * sizeof(node*));
			g.v[i]->aSize = 0;
		}

		for (int i =0; i < e; i++)
		{
			int src = rand() % g.n;
			int dest = rand() % g.n;

			int ok = 0;
			for (int j = 0; j < g.v[src]->aSize; j++)
				if (g.v[src]->a[j] == g.v[dest])
					ok = 1;
			if (ok == 0)
			{
				g.v[src]->a[g.v[src]->aSize++] = g.v[dest];

			}
			if (ok == 1)
				i--;
		}
		List* list = createList();
		int ciclu = 0;
		Operation op = pro.createOperation("DFS-muchii", g.m);
		DFS(&g, list, &ciclu, op);
		free_graph(&g);
	}

	//E=4500
	for (int vf = 100; vf <= 200; vf += 10)
	{
		graph g;
		g.n =vf;
		g.m = 4500;

		g.v = (node**)calloc(g.n, sizeof(node*));
		for (int i = 0; i < g.n; i++)
		{
			g.v[i] = (node*)malloc(sizeof(node));
			g.v[i]->key = i + 1;
			g.v[i]->a = (node**)malloc(g.m * sizeof(node*));
			g.v[i]->aSize = 0;
		}

		for (int i = 0; i < 4500; i++)
		{
			int src = rand() % g.n;
			int dest = rand() % g.n;

			int ok = 0;
			for (int j = 0; j < g.v[src]->aSize; j++)
				if (g.v[src]->a[j] == g.v[dest])
					ok = 1;
			if (ok == 0)
			{
				g.v[src]->a[g.v[src]->aSize++] = g.v[dest];

			}
			if (ok == 1)
				i--;
		}
		List* list = createList();
		int ciclu = 0;
		Operation op = pro.createOperation("DFS-varfuri", vf);
		DFS(&g, list, &ciclu, op);
		free_graph(&g);
	}



	pro.showReport();
}


int main() 
{
	demo();
	//perf();
	return 0;
}