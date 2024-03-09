/*
    In the first part of the assignment, I implemented both a recursive and an iterative algorithm for traversing a binary tree in INORDER. The iterative function has 
    constant additional memory and a complexity of O(n). From the generated graph, we can observe that both implementations have the same number of operations (since 
    we count n prints).

    In the second part of the assignment, I implemented the QuickSort algorithm and QuickSort Hybrid, using iterative InsertionSort for a given threshold. 
    From the generated graph, we can see that the two implementations approach each other in terms of the number of operations, but the classic QuickSort variant 
    proves to be more efficient in this regard. However, in terms of runtime, we observe the opposite: Classic QuickSort is slower than the Hybrid variant.

    Finally, I studied the optimal threshold in hybridization and generated two graphs to ensure the correctness of the result. On the graph named "Prag_100"  
    it is clear that the optimum is found at 9, and the same is confirmed on the graph "Prag_10000".
*/

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"
Profiler pro("Lab8");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 100

typedef struct node
{
    int key;
    node* left;
    node* right;
    node* parent;
} node;

void BUILD_TREE(node** root, int key, node* father)
{
    if (*root != NULL)
    {
        if (key < (*root)->key)
            BUILD_TREE(&(*root)->left, key, *root);
        else
            BUILD_TREE(&(*root)->right, key, *root);
    }
    else if (*root == NULL)
    {
        *root = (node*)malloc(sizeof(node));
        (*root)->left = NULL;
        (*root)->right = NULL;
        (*root)->key = key;
        (*root)->parent = father;
    }
}

void inordine_recursiv(node* root,int n)
{
    Operation op = pro.createOperation("Traversare_recursiva", n);
    if (root != NULL)
    {
        inordine_recursiv(root->left, n);
        op.count();
        printf("%d ", root->key);
        inordine_recursiv(root->right, n);
    }
}

void inordine_iterativ(node* root, int n)
{
    if (root == NULL)
        return;
    Operation op = pro.createOperation("Traversare_iterativa", n);
    node* actual = root;
    int d = 1;

    do
    {
        if (d == 1)
        {
            //preordine
            if (actual->left != NULL)
                actual = actual->left;
            else
                d = 2;
        }
        if (d == 2)
        {
            printf("%d ", actual->key); //inordine
            op.count();
            if (actual->right != NULL)
            {
                actual = actual->right;
                d = 1;
            }
            else
                d = 3;
        }
        if (d == 3)
        {
            //postordine
            if (actual->parent != NULL)
            {
                if (actual == actual->parent->left)
                    d = 2;
                actual = actual->parent;
            }
        }
    } while (!(actual == root && d == 3));

}
/*void inordine_iterativ(node* root)    //MORRIS algh
{
    if (root != NULL)
    {
        node* actual = root;
        while (actual != NULL) //parcurg o singura data nodurile O(n)
        {
            if (actual->left == NULL)       //nu are subarbore stang
            {
                printf("%d ", actual->key);
                actual = actual->right;     //se muta in subarborele drept
            }
            else
            {
                node* before = actual->left;
                while (before->right != NULL && before->right != actual) //caut predecesorul nodului curent
                    before = before->right;

                if (before->right == NULL)  //nu exista legatura dreapta a predecesorului
                {
                    before->right = actual; //nodul curent devine copilul drept al propriului predecesor
                    actual = actual->left;  //se muta mai departe in subarborele stang
                }
                else
                {
                    before->right = NULL;  //se anuleaza legatura
                    printf("%d ", actual->key);
                    actual = actual->right; //se muta mai departe in subarborele drept
                }
            }

        }
    }
}*/

void insertion_sort(int* v, int n, int m, Operation &op)
{
    int aux = 0, i, j;
    for (i = 1; i < n; i++)		//primele i numere vor fi mereu sortate
    {
        op.count();
        aux = v[i];
        j = i - 1;
        op.count();
        while (j >= 0 && v[j] > aux)	//shiftez elem mai mari decat aux din partea sortata (dreapta aux)
        {
            op.count(2);
            v[j + 1] = v[j];
            j--;
        }
        op.count();
        v[j + 1] = aux;		//introduc in pozitia corecta aux
    }
}

int partition(int* v, int n, int p, int r, Operation& op)
{
    int x = v[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; j++)
    {
        op.count();
        if (v[j] <= x)
        {
            i++;
            op.count(3);
            int aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }
    }
    op.count(3);
    int aux = v[i + 1];
    v[i + 1] = v[r];
    v[r] = aux;

    return i + 1;
}

void quicksort_hybrid(int* v, int n, int p, int r, int prag, Operation &op)
{
    int q = 0;
    if(r> p)
    {
        if (r - p + 1 <= prag)
        {
            insertion_sort(v + p, r - p + 1, n, op);
        }
        else
        {
            q = partition(v, n, p, r, op);
            quicksort_hybrid(v, n, p, q - 1, prag, op);
            quicksort_hybrid(v, n, q + 1, r, prag, op);
        }
    }
}
void quicksort(int* v, int n, int p, int r)
{
    Operation op = pro.createOperation("Quick", n);
    int q = 0;
    if (r > p)
    {
      q = partition(v, n, p, r, op);
      quicksort(v, n, p, q - 1);
      quicksort(v, n, q + 1, r);   
    }
}

void demo()
{
    int n = 6;
    //node* root = (node*)malloc(sizeof(node));
    node* root = NULL;
    BUILD_TREE(&root, 13, NULL);
    BUILD_TREE(&root, 11, NULL);
    BUILD_TREE(&root, 9, NULL);
    BUILD_TREE(&root, 12, NULL);
    BUILD_TREE(&root, 16, NULL);
    BUILD_TREE(&root, 19, NULL);

    //node* root = BUILD_TREE(1, n, NULL, n);
    printf("Parcurgere recursiva in inordine: ");
    inordine_recursiv(root, n);

    printf("\n\n");

    printf("Parcurgere iterativa in inordine: ");
    inordine_iterativ(root, n);
    ////////////////////////////////////////////
    printf("\n\n");
    printf("Quicksort: ");

    int m = 10;
    int w[] = { 1,9,23,13,3,2,15,10,20,7 };
    quicksort(w, m, 0, m - 1);
    for (int i = 0; i < m; i++)
        printf("%d ", w[i]);

    printf("\n\n");
    printf("Quicksort hibrid: ");

    Operation op = pro.createOperation("Quick_hibrid", n);
    int v[] = { 1,9,23,13,3,2,15,10,20,7 };
    quicksort_hybrid(v, m, 0, m-1, 30, op);
    for (int i = 0; i < m; i++)
        printf("%d ", v[i]);
    
    printf("\n\n");
}

void perf()
{
    for (int n = 100; n <= MAX_SIZE; n+=STEP_SIZE)
    {
        int* v = (int*)calloc(n, sizeof(int));
        FillRandomArray(v, n, 1, 10000, 1, 0);

        int k = rand() % n;           //consider radacina elementul v[k]
        node* root = NULL;
        BUILD_TREE(&root, v[k], NULL);

        for (int i = 0; i < n; i++)
            if(i!=k)
                BUILD_TREE(&root, v[i], NULL);
        inordine_recursiv(root, n);
        inordine_iterativ(root, n);

        
       for(int j=1; j<NR_TESTS; j++)
       {
           int* v = (int*)calloc(n, sizeof(int));
           int* w = (int*)calloc(n, sizeof(int));
           FillRandomArray(v, n, 1, 90000, 1, 0);
           for (int i = 0; i < n; i++)
               w[i] = v[i];
           quicksort(v, n, 0, n - 1);
           Operation op = pro.createOperation("Quick_hibrid", n);
           quicksort_hybrid(w, n, 0, n - 1, 30, op);
       }
    }
    pro.createGroup("Traversari", "Traversare_iterativa", "Traversare_recursiva");
   
    pro.divideValues("Quick", NR_TESTS);
    pro.divideValues("Quick_hibrid", NR_TESTS);
    pro.createGroup("Quick_Compar", "Quick", "Quick_hibrid");
}

void run_time() 
{
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) 
    {
        int* v = (int*)calloc(n, sizeof(int));
        int* w = (int*)calloc(n, sizeof(int));

        pro.startTimer("quicksort", n);
        for (int k = 0; k <= 100; k++) 
        {
            FillRandomArray(v, n, 1, 90000, 1, 0);
            quicksort(v, n, 0, n - 1);
        }
        pro.stopTimer("quicksort", n);


        pro.startTimer("quicksort_hybrid", n);
        Operation op = pro.createOperation("Quick_hibrid", n);
        for (int j = 0; j <= 100; j++)
        {
            FillRandomArray(w, n, 1, 90000, 1, 0);
            quicksort_hybrid(w, n, 0, n - 1, 30, op);
        }
        pro.stopTimer("quicksort_hybrid", n);
    }
    pro.createGroup("Runtime", "quicksort", "quicksort_hybrid");
}

void prag_optim()
{
    int n = 10000;
    int* v = (int*)calloc(n, sizeof(int));
    FillRandomArray(v, n, 1, 90000, 1, 0);

    for (int i = 0; i < 100; i++)
    {
        int* w = (int*)calloc(n, sizeof(int));

        for (int j = 0; j < n; j++)
            w[j] = v[j];

        Operation op = pro.createOperation("Prag_100", i);
        quicksort_hybrid(w, n, 0, n - 1, i, op);
    }
    for (int i = 0; i < n; i++)
    {
        int* w = (int*)calloc(n, sizeof(int));

        for (int j = 0; j < n; j++)
            w[j] = v[j];

        Operation op = pro.createOperation("Prag_10000", i);
        quicksort_hybrid(w, n, 0, n - 1, i, op);
    }
    //pro.showReport();
}


void allperf()
{
    perf();
    run_time();
    prag_optim();
    pro.showReport();
}

int main()
{
    demo();
    //allperf();

    return 0;
}
