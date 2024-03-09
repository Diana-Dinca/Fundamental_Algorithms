/*
	I implemented two functions for building a heap using two methods (BottomUp O(n) and TopDown O(nlogn)), and a function for sorting the heap O(nlog n). 
	I calculated the number of comparisons and assignments for each build_heap operation. For the average case, I observed that the BottomUp implementation is more efficient, 
	showing a smaller growth coefficient than TopDown. This observation remains consistent for the worst-case scenario, with the difference in efficiency between the two 
	algorithms being much more evident. In conclusion, building a heap with BottomUp is favorable.
*/
#include<stdio.h>
#include<stdlib.h>
#include "Profiler.h"
#define nr_tests 5

Profiler p("Heap");

void heapify_BottomUp(int* v, int f, int n)
{
	Operation opAtr = p.createOperation("BottomUp_atr", n);
	Operation opCom = p.createOperation("BottomUp_comp", n);
	int max = f;
	int l = 2 * f + 1;
	int r = 2 * f + 2;
	//aflu nodul cu valoare maxima
	opCom.count();	
	if (l < n && v[l] > v[max])
		max = l;
	opCom.count();
	if (r < n && v[r] > v[max]) 
		max = r;		
	//devine parinte nodul maxim
	if (max != f)
	{
		opAtr.count(3);
		int aux = v[f];
		v[f] = v[max];
		v[max] = aux;
		heapify_BottomUp(v, max, n);
	}
}

void build_heap_BottomUp(int* v, int n)
{
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify_BottomUp(v, i, n);
}

void heapify_TopDown(int* v, int c, int n)
{
	Operation opAtr = p.createOperation("TopDown_atr", n);
	Operation opCom = p.createOperation("TopDown_comp", n);
	int r = 0;
	if (c % 2 == 1)
		r = (c - 1) / 2;//indice impar=> copil stang
	else
		r = (c - 2) / 2;//indice par=> copil drept

	if (r >= 0)
	{
		opCom.count();
		if (v[c] > v[r])
		{
			opAtr.count(3);
			int aux = v[r];
			v[r] = v[c];
			v[c] = aux;
			heapify_TopDown(v, r, n);
		}
	}
}

void build_heap_TopDown(int* v, int n)
{
	for (int i = 1; i < n; i++)
		heapify_TopDown(v, i, n);
}

void heap_sort(int* v, int n)
{
	build_heap_BottomUp(v, n);
	for (int i = n - 1; i >= 1; i--)
	{
		int aux = v[0];
		v[0] = v[i];
		v[i] = aux;
		n--;
		heapify_BottomUp(v, 0, n);
	}
}

void demo()
{
	int const n = 10;
	int v1[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	int v2[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	int v3[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	build_heap_BottomUp(v1, n);
	build_heap_TopDown(v2, n);
	heap_sort(v3, n);

	printf("BottomUp: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v1[i]);

	printf("\n\nTopDown: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v2[i]);

	printf("\n\nHeapSort: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v3[i]);

	printf("\n");
}

void performance(int order)
{
	int* v, * v1, * v2, * v3;
	for (int n = 100; n <= 10000; n += 100)
		for (int i = 1; i <= nr_tests; i++)
		{
			v = (int*)calloc(n, sizeof(int));
			v1 = (int*)calloc(n, sizeof(int));

			FillRandomArray(v, n, 100, 10000, 0, order);
			for (int j = 0; j < n; j++)
			{
				v1[j] = v[j];
			}
			build_heap_BottomUp(v, n);
			build_heap_TopDown(v1, n);
		}
	p.divideValues("BottomUp_atr", nr_tests);
	p.divideValues("BottomUp_comp", nr_tests);
	p.addSeries("BottomUp_total", "BottomUp_atr", "BottomUp_comp");

	p.divideValues("TopDown_atr", nr_tests);
	p.divideValues("TopDown_comp", nr_tests);
	p.addSeries("TopDown_total", "TopDown_atr", "TopDown_comp");

	p.createGroup("total_atr", "BottomUp_atr", "TopDown_atr");
	p.createGroup("total_comp", "BottomUp_comp", "TopDown_comp");
	p.createGroup("total", "BottomUp_total", "TopDown_total");
}

void all_performace()
{
	p.reset("average");
	performance(0);
	p.reset("worst");
	performance(1);
	p.showReport();
}

int main()
{
    demo();
	//all_performace();

	return 0;
}