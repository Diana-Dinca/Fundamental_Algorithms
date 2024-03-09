/*
	I implemented the QuickSort algorithm with the pivot element at the end, QuickSort with random pivot, and QuickSort with the pivot element in the middle. 
	Additionally, I implemented the HeapSort algorithm and compared it with QuickSort in the average case, where I observed the efficiency of QuickSort.

	In the worst case, QuickSort exhibits polynomial growth and a complexity of O(n^2), while in the best case, QuickSort with the pivot element in the middle has a 
	complexity of O(nlog n).

	I also implemented BinaryInsertionSort both iteratively and recursively, noticing that for all cases - average (polynomial growth), best (linear growth), 
	and worst (polynomial growth) - the efficiency remains consistent between the two algorithms.

	Ultimately, the runtime between the iterative and recursive approaches proves to be more efficient in the iterative case.
*/


#include<stdio.h>
#include<stdlib.h>
#include "Profiler.h"
#define nr_tests 5
#define MAX_SIZE 10000
#define STEP_SIZE 100


Profiler pro("Heap");
//pivot final
int partition(int* v,int n, int p, int r)
{
	Operation opAtr = pro.createOperation("Quick_atr", n);
	Operation opCom = pro.createOperation("Quick_com", n);
	opAtr.count();
	int x = v[r];
	int i = p - 1;
	for (int j = p; j <= r - 1; j++)
	{
		opCom.count();
		if (v[j] <= x)
		{
			i++;

			opAtr.count(3);

			int aux = v[i];
			v[i] = v[j];
			v[j] = aux;
		}
	}
	opAtr.count(3);

	int aux = v[i+1];
	v[i+1] = v[r];
	v[r] = aux;

	return i + 1;
}

void quicksort(int* v,int n, int p, int r)
{
	int q = 0;
	if (p < r)
	{
		q = partition(v,n, p, r);
		quicksort(v,n, p, q -1 );
		quicksort(v,n, q + 1, r);
	}
}
//pivot middle
int partition_mid(int* v, int n, int p, int r)
{
	Operation opAtr = pro.createOperation("Quick_mid_atr", n);
	Operation opCom = pro.createOperation("Quick_mid_com", n);
	opAtr.count();
	int x = v[(p + r) / 2];
	int i = p, j = r;

	int aux = v[r];
	v[r] = v[(p + r) / 2];
	v[(p + r) / 2] = aux;

	while (j >= i)
	{
		while (v[i] < x)
		{
			i++;
			opCom.count();
		}
		opCom.count();
		while (v[j] > x)
		{
			j--;
			opCom.count();
		}
		opCom.count();

		if (i <= j)
		{
			opAtr.count(3);
			int aux = v[i];
			v[i] = v[j];
			v[j] = aux;
			i++;
			j--;
		}
	}
	
	return j;
}
void quicksort_mid(int* v, int n, int p, int r)
{
	int q = 0;
	if (p < r)
	{
		q = partition_mid(v, n, p, r);
		quicksort_mid(v, n, p, q);
		quicksort_mid(v, n, q + 1, r);
	}
}

// pivot random
int random_partition(int* v,int n, int p, int r)
{
	int i = rand()%(r-p+1)+p;
	int aux = v[i];
	v[i] = v[r];
	v[r] = aux;
	return partition(v,n, p, r);
}

void random_quicksort(int* v, int n, int p, int r)
{
	int q = 0;
	if (p < r)
	{
		q = random_partition(v, n, p, r);
		random_quicksort(v,n, p, q - 1);
		random_quicksort(v,n, q + 1, r);
	}
}

void heapify_BottomUp(int* v, int f, int nn, int n)
{
	Operation opAtr = pro.createOperation("HeapSort_atr", n);
	Operation opCom = pro.createOperation("HeapSort_com", n);

	int max = f;
	int l = 2 * f + 1;
	int r = 2 * f + 2;
	//aflu nodul cu valoare maxima
	opCom.count();
	if (l < nn && v[l] > v[max])
		max = l;
	opCom.count();
	if (r < nn && v[r] > v[max])
		max = r;
	//devine parinte nodul maxim
	if (max != f)
	{
		opAtr.count(3);
		int aux = v[f];
		v[f] = v[max];
		v[max] = aux;
		heapify_BottomUp(v, max, nn, n);
	}
}

void build_heap_BottomUp(int* v, int nn, int n)
{
	for (int i = nn / 2 - 1; i >= 0; i--)
		heapify_BottomUp(v, i, nn, n);
}

void heap_sort(int* v, int n, int nn)
{
	Operation opAtr = pro.createOperation("HeapSort_atr", n);

	build_heap_BottomUp(v, nn, n);
	for (int i = nn - 1; i >= 1; i--)
	{
		opAtr.count(3);
		int aux = v[0];
		v[0] = v[i];
		v[i] = aux;
		nn--;
		heapify_BottomUp(v, 0, nn, n);
	}
}

///////////////////////////////////////////////////////////////////////// Sort lab1 iterativa
void binary_insertion_sort(int* v, int n)
{
	Operation opComp = pro.createOperation("BinaryInsertion_com", n);
	Operation opAttr = pro.createOperation("BinaryInsertion_atr", n);
	int aux = 0, i, j, poz = 0;

	for (i = 1; i < n; i++)
	{
		opAttr.count();
		aux = v[i];
		j = i - 1;
		int st = 0, dr = j; 
		int mij = (st + dr) / 2;

		while (st <= dr)
		{
			opComp.count();
			if (aux <= v[mij])
				dr = mij - 1;
			else
				st = mij + 1;
			mij = (st + dr) / 2;
		} //pozitia unde trebuie inserat

		while (j >= st)	//shiftez elem din partea sortata
		{
			opAttr.count();
			v[j + 1] = v[j];
			j--;
		}
		opAttr.count();
		v[st] = aux;		//introduc in pozitia corecta aux
	}
}


///////////////////////////////////////////////////////////////////////// Sort lab1 recursiva
int binary_search_rec(int* v, int n, int x, int st, int dr)
{
	Operation opComp = pro.createOperation("BinaryInsertionRec_com", n);
	if (st <= dr)
	{
		int mij = (st + dr) / 2;

		opComp.count();
		if (x <= v[mij])
			return binary_search_rec(v, n, x, st, mij - 1);
		return binary_search_rec(v, n, x, mij + 1, dr);
	}
	return st;
}

void binary_insertion_sort_rec(int* v, int n, int i)
{
	Operation opComp = pro.createOperation("BinaryInsertionRec_com", n);
	Operation opAttr = pro.createOperation("BinaryInsertionRec_atr", n);
	if (i < n)
	{
		opAttr.count();
		int aux = v[i];
		int j = i - 1;
		int poz = binary_search_rec(v,n, aux, 0, j); //pozitia unde trebuie inserat
		
		if (poz >=0 && poz < n)
		{
			while (j >= poz)	//shiftez elem din partea sortata
			{
				opAttr.count();
				v[j + 1] = v[j];
				j--;
			}
			opAttr.count();
			v[poz] = aux;		//introduc in pozitia corecta aux
		}
		binary_insertion_sort_rec(v, n, i + 1);
	}
	return;
}

void demo()
{
	int const n = 10;
	int v[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	int v1[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	int v2[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	int v3[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	int v4[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	int v5[n] = { 1, 10, 7, 3, 2, 5, 9, 8, 4, 6 };
	
	quicksort(v, n, 0, n-1);
	random_quicksort(v1, n, 0, n-1);
	quicksort_mid(v5, n, 0, n-1);
	heap_sort(v2, n, n);
	binary_insertion_sort(v3, n);
	binary_insertion_sort_rec(v4, n, 0);

	printf("QuickSort: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
	printf("\n\nRandomQuickSort: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v1[i]);
	printf("\n\nQuickSort_Middle: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v5[i]);
	printf("\n\nHeapSort: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v2[i]);
	printf("\n\nBinarySearchSort: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v3[i]);
	printf("\n\nBinarySearchSort_rec: ");
	for (int i = 0; i < n; i++)
		printf("%d ", v4[i]);
	printf("\n\n");
}

void performanceAverage(int order)
{
	//Quick vs Heap
	//Iterativ vs Recursiv
	int* v, * v1, * v2, * v3, * v4;
	for (int n = 100; n <= 10000; n += 100)
		for (int i = 1; i <= nr_tests; i++)
		{
			int nn = n;
			v = (int*)calloc(n, sizeof(int));
			v1 = (int*)calloc(n, sizeof(int));
			v2 = (int*)calloc(n, sizeof(int));
			v3 = (int*)calloc(n, sizeof(int));
						
			FillRandomArray(v, n, 100, 10000, 0, order);
			for (int j = 0; j < n; j++)
			{
				v1[j] = v[j];
				v2[j] = v[j];
				v3[j] = v[j];				
			}
			quicksort(v, n, 0, n - 1);
			heap_sort(v1, n, nn);
			binary_insertion_sort(v2, n);
			binary_insertion_sort_rec(v3, n, 0);
		}
	pro.divideValues("Quick_atr", nr_tests);
	pro.divideValues("Quick_com", nr_tests);
	pro.addSeries("Quick_total", "Quick_atr", "Quick_com");
	
	pro.divideValues("HeapSort_atr", nr_tests);
	pro.divideValues("HeapSort_com", nr_tests);
	pro.addSeries("HeapSort_total", "HeapSort_atr", "HeapSort_com");

	// interpretare QuickSort vs HeapSort
	pro.createGroup("QuickVsHeap_atr", "Quick_atr", "HeapSort_atr");
	pro.createGroup("QuickVsHeap_com", "Quick_com", "HeapSort_com");
	pro.createGroup("QuickVsHeap_total", "Quick_total", "HeapSort_total");
		

	pro.divideValues("BinaryInsertion_atr", nr_tests);
	pro.divideValues("BinaryInsertion_com", nr_tests);
	pro.addSeries("BinaryInsertion_total", "BinaryInsertion_atr", "BinaryInsertion_com");

	pro.divideValues("BinaryInsertionRec_atr", nr_tests);
	pro.divideValues("BinaryInsertionRec_com", nr_tests);
	pro.addSeries("BinaryInsertionRec_total", "BinaryInsertionRec_atr", "BinaryInsertionRec_com");

	// interpretare versiune iterativa vs recursiva
	pro.createGroup("IterativVsRecursiv_atr", "BinaryInsertion_atr", "BinaryInsertionRec_atr");
	pro.createGroup("IterativVsRecursiv_com", "BinaryInsertion_com", "BinaryInsertionRec_com");
	pro.createGroup("IterativVsRecursiv_total", "BinaryInsertion_total", "BinaryInsertionRec_total");
	
}
void performanceWorst(int order)
{
	//Quick
	//Iterativ vs Recursiv
	int* v, * v1, * v2, * v3, * v4;
	for (int n = 100; n <= 10000; n += 100)
		for (int i = 1; i <= nr_tests; i++)
		{
			int nn = n;
			v = (int*)calloc(n, sizeof(int));
			v2 = (int*)calloc(n, sizeof(int));
			v3 = (int*)calloc(n, sizeof(int));

			FillRandomArray(v, n, 100, 10000, 0, order);
			for (int j = 0; j < n; j++)
			{
				v2[j] = v[j];
				v3[j] = v[j];
			}
			quicksort(v, n, 0, n - 1);
			binary_insertion_sort(v2, n);
			binary_insertion_sort_rec(v3, n, 0);
		}
	// Quick
	pro.divideValues("Quick_atr", nr_tests);
	pro.divideValues("Quick_com", nr_tests);
	pro.addSeries("Quick_total", "Quick_atr", "Quick_com");


	pro.divideValues("BinaryInsertion_atr", nr_tests);
	pro.divideValues("BinaryInsertion_com", nr_tests);
	pro.addSeries("BinaryInsertion_total", "BinaryInsertion_atr", "BinaryInsertion_com");

	pro.divideValues("BinaryInsertionRec_atr", nr_tests);
	pro.divideValues("BinaryInsertionRec_com", nr_tests);
	pro.addSeries("BinaryInsertionRec_total", "BinaryInsertionRec_atr", "BinaryInsertionRec_com");

	// interpretare versiune iterativa vs recursiva
	pro.createGroup("IterativVsRecursiv_atr", "BinaryInsertion_atr", "BinaryInsertionRec_atr");
	pro.createGroup("IterativVsRecursiv_com", "BinaryInsertion_com", "BinaryInsertionRec_com");
	pro.createGroup("IterativVsRecursiv_total", "BinaryInsertion_total", "BinaryInsertionRec_total");

}

void performanceBest(int order)
{
	//Quick pivot mijloc
	//Iterativ vs Recursiv
	int* v, * v1, * v2, * v3, * v4;
	for (int n = 100; n <= 10000; n += 100)
		for (int i = 1; i <= nr_tests; i++)
		{
			int nn = n;
			v = (int*)calloc(n, sizeof(int));
			v2 = (int*)calloc(n, sizeof(int));
			v3 = (int*)calloc(n, sizeof(int));

			FillRandomArray(v, n, 100, 10000, 0, order);
			for (int j = 0; j < n; j++)
			{
				v2[j] = v[j];
				v3[j] = v[j];
			}
			quicksort_mid(v, n, 0, n - 1);
			binary_insertion_sort(v2, n);
			binary_insertion_sort_rec(v3, n, 0);
		}
	//Quick
	pro.divideValues("Quick_mid_atr", nr_tests);
	pro.divideValues("Quick_mid_com", nr_tests);
	pro.addSeries("Quick_mid_total", "Quick_mid_atr", "Quick_mid_com");


	pro.divideValues("BinaryInsertion_atr", nr_tests);
	pro.divideValues("BinaryInsertion_com", nr_tests);
	pro.addSeries("BinaryInsertion_total", "BinaryInsertion_atr", "BinaryInsertion_com");

	pro.divideValues("BinaryInsertionRec_atr", nr_tests);
	pro.divideValues("BinaryInsertionRec_com", nr_tests);
	pro.addSeries("BinaryInsertionRec_total", "BinaryInsertionRec_atr", "BinaryInsertionRec_com");

	//Interpretare versiune iterativa vs recursiva
	pro.createGroup("IterativVsRecursiv_atr", "BinaryInsertion_atr", "BinaryInsertionRec_atr");
	pro.createGroup("IterativVsRecursiv_com", "BinaryInsertion_com", "BinaryInsertionRec_com");
	pro.createGroup("IterativVsRecursiv_total", "BinaryInsertion_total", "BinaryInsertionRec_total");

}

void run_time() {
	int v1[MAX_SIZE],v2[MAX_SIZE];
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		pro.startTimer("Binary insertion", n);
		for (int k = 0; k <= 100; k++) {
			FillRandomArray(v1, n, 10, 50000, false, 0);
			binary_insertion_sort(v1, n);
		}
		pro.stopTimer("Binary insertion", n);


		pro.startTimer("Binary insertion Rec", n);
		for (int j = 0; j <= 50; j++) {
			FillRandomArray(v2, n, 10, 50000, false, 0);
			binary_insertion_sort_rec(v2, n, 1);
		}
		pro.stopTimer("Binary insertion Rec", n);
	}
	pro.createGroup("Runtime iterative vs recursive", "Binary insertion", "Binary insertion Rec");
}


void all_performace()
{
	pro.reset("Average");
	performanceAverage(0);

	pro.reset("Worst");
	performanceWorst(2);
	
	pro.reset("Best");
	performanceBest(1);
	
	run_time();
	pro.showReport();
}


int main()
{
    demo();
	//all_performace();
	return 0;
}