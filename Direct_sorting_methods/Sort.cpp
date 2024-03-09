/*
I implemented 3 sorting algorithms: Bubble Sort, Insertion Sort, and Selection Sort, as well as the Binary Insertion Sort algorithm using binary search.
The "demo" function presents a simple case demonstrating the correct functioning of the sorting algorithms.
The "all_performance" function assists in generating graphs by calling the "performance" function, which takes the parameter order={0,1,2} to distinguish 
between the average, best, and worst cases.

In terms of assignments, the Bubble Sort, Insertion Sort, and Binary Insertion Sort algorithms grow polynomially, with Bubble Sort having the highest rate, 
while Selection Sort has a linear growth. In terms of comparisons, Bubble Sort and Selection Sort grow polynomially with a much higher rate than 
Insertion Sort and Binary Insertion Sort.
In the end, Bubble Sort is the most costly algorithm, while Selection and Insertion tend to have similar levels, and Binary Insertion is more efficient.

Regarding assignments, Bubble Sort and Selection Sort are the most efficient, while Insertion Sort and Binary Insertion show very close linear growth rates.
In terms of comparisons, Bubble Sort and Insertion Sort have similar linear growth, while Binary Selection Sort and Selection Sort have polynomial growth.
Ultimately, Bubble Sort is the most efficient, followed by Insertion Sort, Binary Insertion Sort with linear growth, and Selection Sort with unfavorable polynomial growth.

For assignments, Selection Sort has linear growth, while Insertion Sort and Binary Insertion Sort have polynomial growth with a smaller factor than Bubble Sort.
In terms of comparisons, all algorithms show polynomial growth, with a slightly differentiable factor between them, but the most efficient being Binary Insertion Sort.
In the end, Selection Sort is the most efficient, with minimal polynomial growth, followed by Binary Insertion Sort, Insertion Sort, and Bubble Sort being the least efficient.
*/

#define _CTR_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

#define nr_tests 5

Profiler p("Sort");

int binary_search(int *v, int n, int x, int st, int dr)
{
	Operation opComp = p.createOperation("binary_insertion_comp", n);

	int mij = (st + dr) / 2;
	
	while (st <= dr)
	{
		opComp.count();
		if (x <= v[mij])
			dr = mij - 1;
		else
			st = mij + 1;
		mij = (st + dr) / 2;
	}
	return st;
}

void binary_insertion_sort(int* v, int n)
{
	Operation opComp = p.createOperation("binary_insertion_comp", n);
	Operation opAttr = p.createOperation("binary_insertion_attr", n);
	int aux = 0, i, j, poz=0;

	for (i = 1; i < n; i++)
	{
		opAttr.count();
		aux = v[i];
		j = i - 1;
		poz = binary_search(v, n, aux, 0, j); //pozitia unde trebuie inserat
		
		while (j >= poz)	//shiftez elem din partea sortata
		{
			opAttr.count();
			v[j + 1] = v[j];
			j--;
		}

		opAttr.count();
		v[poz] = aux;		//introduc in pozitia corecta aux
	}
}


void selection_sort(int* v, int n)
{
	Operation opComp = p.createOperation("selection_comp", n);
	Operation opAttr = p.createOperation("selection_attr", n);
	int aux = 0, p=0, i, j;

	for (i = 0; i < n-1; i++)
	{
		p = i;					//retin pozitia minimului
		for(j=i+1; j< n; j++)
		{
			opComp.count();
			if (v[p] > v[j])	//caut un element mai mic => retin pozitia
				p = j;
		}
		if (p != i)		//am un elem mai mic => interschimb
		{
			opAttr.count(3);
			aux = v[i];
			v[i] = v[p];
			v[p] = aux;
		}
	}
}

void insertion_sort(int* v, int n)
{
	Operation opComp = p.createOperation("insertion_comp", n);
	Operation opAttr = p.createOperation("insertion_attr", n);
	int aux = 0, i, j;

	for (i = 1; i < n; i++)		//primele i numere vor fi mereu sortate
	{
		opAttr.count();
		aux = v[i];			
		j = i - 1;
		while (j >= 0 && v[j] > aux)	//shiftez elem mai mari decat aux din partea sortata (dreapta aux)
		{
			opComp.count();
			opAttr.count();
			v[j + 1] = v[j];
			j--;
		}
		opComp.count();
		opAttr.count();
		v[j + 1] = aux;		//introduc in pozitia corecta aux
	}
}


void bubble_sort(int* v, int n)
{
	Operation opComp = p.createOperation("bubble_comp", n);
	Operation opAttr = p.createOperation("bubble_attr", n);
	int aux = 0, ok = 0;

	for (int i = 0; i < n; i++)		//dupa fiecare iteratie avem maximul pe ultima pozitie 
	{								//ultimele i numere vor fi mereu sortate
		int ok = 0;
		for (int j = 0; j < n - i - 1; j++) 
		{
			opComp.count();
			if (v[j] > v[j + 1])	//punem elem vecine in ordine 
			{
				opAttr.count(3);
				aux = v[j];
				v[j] = v[j + 1];
				v[j + 1] = aux;
				ok = 1;
			}
		}
		if (ok == 0)
			break;
	}

}

void demo()
{
	int n = 10;
	int v[] = { 2,7,1,5,4,6,9,8,3,11 };
	//bubble_sort(v, n);
	//insertion_sort(v, n);
	//selection_sort(v, n);
	binary_insertion_sort(v, n);
	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
}

void performance(int order)
{	
	int *v, *v1, *v2, *v3;
	for (int n = 100; n <= 10000; n += 100)
		for(int i=1; i<=nr_tests; i++)
		{
			v = (int*)calloc(n, sizeof(int));
			v1 = (int*)calloc(n, sizeof(int));
			v2 = (int*)calloc(n, sizeof(int));
			v3 = (int*)calloc(n, sizeof(int));
			FillRandomArray(v, n, 100, 10000, 0, order);
			for (int j = 0; j < n; j++)
			{
				v1[j] = v[j];
				v3[j] = v[j];
				if (order == 2) //worst case pt selsction 9 1 2 3 4 5 6 7 8
				{
					if (j == 0)
						v2[j] = v[j];
					else
						v2[n - j] = v[j];
				}
				else
					v2[j] = v[j];

			}
			bubble_sort(v, n);
			insertion_sort(v1, n);
			selection_sort(v2, n);
			binary_insertion_sort(v3, n);
		}
	p.divideValues("bubble_comp", nr_tests);
	p.divideValues("bubble_attr", nr_tests);
	p.addSeries("bubble", "bubble_comp", "bubble_attr");

	p.divideValues("insertion_comp", nr_tests);
	p.divideValues("insertion_attr", nr_tests);
	p.addSeries("insertion", "insertion_comp", "insertion_attr");

	p.divideValues("selection_comp", nr_tests);
	p.divideValues("selection_attr", nr_tests);
	p.addSeries("selection", "selection_comp", "selection_attr");

	p.divideValues("binary_insertion_comp", nr_tests);
	p.divideValues("binary_insertion_attr", nr_tests);
	p.addSeries("binary_insertion", "binary_insertion_comp", "binary_insertion_attr");


	p.createGroup("comp", "bubble_comp", "insertion_comp", "selection_comp", "binary_insertion_comp");
	p.createGroup("attr", "bubble_attr", "insertion_attr", "selection_attr", "binary_insertion_attr");
	p.createGroup("total", "bubble", "insertion", "selection", "binary_insertion");

}

void all_performance()
{
	p.reset("average");
	performance(0);
	p.reset("best");
	performance(1);
	p.reset("worst");
	performance(2);
	p.showReport();
}

int main()
{
	demo();
	//all_performance();

	return 0;
}