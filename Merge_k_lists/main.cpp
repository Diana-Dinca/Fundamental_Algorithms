/*
	I implemented an algorithm that generates k lists sorted in ascending order, each with different lengths, totaling n elements, an algorithm 
	for merging two lists, and an algorithm for merging k lists using MinHeap. The algorithm's complexity is O(n log k).

	The generation algorithm uses the expression length = rand() % (nn - (k - i - 1)) + 1 to generate a random value for the current list's size, 
	initially nn = n, and with each generation, its value is updated. The function takes into account the remaining number of elements (remaining) 
	and the remaining number of lists (k-i-1) to ensure that the sizes of the generated lists will contribute to obtaining the total sum n.

	The algorithm for merging k lists constructs a Heap with the first element from each of the k lists, forms a MinHeap, and places the root 
	into a final list. In place of the element removed from the Heap, the next element from the same list is placed, forming a MinHeap, and the steps 
	are repeated. When one of the lists becomes NULL, we decrease the number of lists and repeat the merging steps for the remaining ones.

	The graph for varying k three times shows somewhat linear growth and the fact that for a larger k, the number of operations is higher.
	The graph for n=100000 shows logarithmic growth.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler pro("Merge_k_lists");
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 15

typedef struct nod{
	int key;
	int index;
	struct nod* next;
}Node;

typedef struct Lista {
	Node* first;
	Node* last;
	int size;
}List;

typedef struct Heap {
	int index;
	int key;
}Heap;

Lista* createList()
{
	List* l = (List*)malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	return l;
}

Node* create(int givenkey, int i)
{
	Node* p = (Node*)malloc(sizeof(Node));
	p->key = givenkey;
	p->next = NULL;
	p->index = i;
	return p;
}


void insert_last_generare(List* l, int givenkey, int i)
{
	Node* p = create(givenkey, i);
	if (l->last == NULL) //list is empty
	{
		l->first = p;
		l->last = p;
	}
	else //there are elements in the list
	{
		l->last->next = p;
		l->last = p;
	}
	l->size++;
}
void insert_last(List* l, int givenkey, int i, Operation& opComp, Operation& opAttr)
{
	Node* p = create(givenkey,i);
	opComp.count();
	if (l->last == NULL)
	{
		opAttr.count();
		l->first = p;
		l->last = p;
	}
	else
	{
		opAttr.count();
		l->last->next = p;
		l->last = p;
	}
	l->size++;
}

void heapify_BottomUp(Heap* v, int f, int n, Operation& opComp, Operation& opAttr)
{
	int smallest = f;
	int l= 2 * f + 1;
	int r = 2 * f + 2;
		
	if (l < n) 
	{
		opComp.count();
		if (v[l].key < v[smallest].key)
			smallest = l;
		else
			smallest = f;
	}
	
	if (r < n)
	{
		opComp.count();
		if(v[r].key < v[smallest].key)
			smallest = r;
	}

	if (smallest != f)
	{
		opAttr.count(3);
		Heap aux = v[f];
		v[f] = v[smallest];
		v[smallest] = aux;
		heapify_BottomUp(v, smallest, n, opComp, opAttr);
	}
}

void build_heap_BottomUp(Heap* v, int n, Operation& opComp, Operation& opAttr)
{
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify_BottomUp(v, i, n,opComp, opAttr);
}

List** generate_list(int n, int k)
{
	int nn = n;
	List** total = (List**)malloc(k*sizeof(List*));

	for (int i = 0; i < k-1; i++)
	{
		int lungime = rand() % (nn - (k - i -1)) + 1;
		
		nn = nn - lungime;

		int* v = (int*)calloc(lungime, sizeof(int));
		FillRandomArray(v, lungime, 0, 10000, 0, 1);

		List* l = createList();
		for (int j = 0; j < lungime; j++)
			insert_last_generare(l, v[j],i);		

		total[i] = l;
	}

	int* v = (int*)calloc(nn, sizeof(int));
	FillRandomArray(v, nn, 0, 10000, 0, 1);

	List* l = createList();
	for (int j = 0; j < nn; j++)
		insert_last_generare(l, v[j],k-1);

	total[k-1] = l;
	return total;
}

List* merge_2lists(List* l1, List* l2)
{
	List* combined = createList();
	//combined->size = l1->size + l2->size;

	Node* p = l1->first;
	Node* q = l2->first;
	while (p != NULL && q != NULL)
	{
		if (p->key < q->key)
		{
			insert_last_generare(combined, p->key, p->index);
			p = p->next;
		}
		else
		{
			insert_last_generare(combined, q->key, q->index);
			q = q->next;
		}
	}
	while (p != NULL)
	{
		insert_last_generare(combined, p->key, p->index);
		p = p->next;
	}
	while (q != NULL)
	{
		insert_last_generare(combined, q->key, q->index);
		q = q->next;
	}
	return combined;
}

List* merge_klists(List** total, int k, int n, Operation& opComp, Operation& opAttr)
{
	Heap* l = (Heap*)calloc(k, sizeof(Heap));
	List* final = createList();                //final merged list
	Node** v = (Node**)calloc(k,sizeof(Node*));//position contor
	Node* q = (Node*)calloc(k, sizeof(Node*));
	int size = k;

	for (int i = 0; i < k; i++)
	{
		opAttr.count(3);
		l[i].key = total[i]->first->key;
		l[i].index = i;
		v[i]=total[i]->first;
	}
	build_heap_BottomUp(l, k, opComp, opAttr);
	
	while (size > 0)
	{
		for (int i = 0; i < n ; i++)
		{
			insert_last(final, l[0].key, l[0].index, opComp, opAttr);
			v[l[0].index] = v[l[0].index]->next;

			opComp.count();
			if (v[l[0].index])
			{
				opAttr.count();
				l[0].key = v[l[0].index]->key; //delete first from the heap and replace 
				//l[0].index = v[l[0].index]->index;
			}
			else
			{ 
				opAttr.count();
				l[0] = l[size-1]; //delete first and replace with the last element from heap
				size--;
			}
			heapify_BottomUp(l, 0, size, opComp, opAttr);
			
		}
	}
	return final;
}

void perf() {
	int k[] = { 5, 10, 100 }, n;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		Operation opComp1 = pro.createOperation("comp_5", n);
		Operation opAttr1 = pro.createOperation("attr_5", n);
		for (int j = 0; j < NR_TESTS; j++) 
		{
			List** total = generate_list(n,k[0]);
			merge_klists(total, k[0], n, opComp1, opAttr1);
		}
	}
	pro.divideValues("comp_5", NR_TESTS);
	pro.divideValues("attr_5", NR_TESTS);
	pro.addSeries("Total_5", "comp_5", "attr_5");


	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		Operation opComp2 = pro.createOperation("comp_10", n);
		Operation opAttr2 = pro.createOperation("attr_10", n);
		for (int j = 0; j < NR_TESTS; j++)
		{
			List** total = generate_list(n, k[1]);
			merge_klists(total, k[1], n, opComp2, opAttr2);
		}
	}
	pro.divideValues("comp_10", NR_TESTS);
	pro.divideValues("attr_10", NR_TESTS);
	pro.addSeries("Total_10", "comp_10", "attr_10");


	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		Operation opComp3 = pro.createOperation("comp_100", n);
		Operation opAttr3 = pro.createOperation("attr_100", n);
		for (int j = 0; j < NR_TESTS; j++)
		{
			List** total = generate_list(n, k[2]);
			merge_klists(total, k[2], n, opComp3, opAttr3);
		}
	}
	pro.divideValues("comp_100", NR_TESTS);
	pro.divideValues("attr_100", NR_TESTS);
	pro.addSeries("Total_100", "comp_100", "attr_100");

	pro.createGroup("atribuiri", "attr_5", "attr_10", "attr_100");
	pro.createGroup("comparatii", "comp_5", "comp_10", "comp_100");
	pro.createGroup("Total", "Total_5", "Total_10", "Total_100");

	pro.reset("PARTEA 2");

	n = MAX_SIZE;
	for (int i = 10; i <= 500; i += 10) 
	{
		int w = i;
		for (int j = 0; j < NR_TESTS; j++) 
		{
			Operation opComp4 = pro.createOperation("comp_10000", i);
			Operation opAttr4 = pro.createOperation("attr_10000", i);

			List** total = generate_list(n, w);
			merge_klists(total, w, n, opComp4, opAttr4);
		}
	}
	pro.divideValues("comp_10000", NR_TESTS);
	pro.divideValues("attr_10000", NR_TESTS);
	pro.addSeries("Total_10000", "comp_10000", "attr_10000");
	pro.createGroup("Total_n_10000", "Total_10000", "comp_10000", "attr_10000");

	pro.showReport();

}


void demo()
{
	int const n = 20;
	int k = 4;

	//Generate k random sorted lists with a total of n elements
	List** total = generate_list(n, k);
	for (int i = 0; i < k; i++)
	{
		Node* p = total[i]->first;
		while (p != NULL)
		{				
			printf("%d ", p->key);
			p = p->next;
		}
		printf("\n\n");
	}

	//Merge 2 lists
	printf("Merge 2 lists: ");
	List* combined= merge_2lists(total[0], total[1]);
	Node* p = combined->first;
	while (p != NULL)
	{
		printf("%d ", p->key);
		p = p->next;
	}
	printf("\n\n");


	//Merge k lists
	Operation opComp = pro.createOperation("compTest", n);
	Operation opAttr = pro.createOperation("attrTest", n);
	printf("Merge k lists: ");
	List* final = merge_klists(total, k, n, opComp, opAttr);
	Node* q = final->first;
	while (q != NULL)
	{
		printf("%d ", q->key);
		q = q->next;
	}
	printf("\n\n");

}

int main()
{
	demo();
	//perf();
	return 0;
}
