/*
    I have implemented the functions MAKE_SET, FIND_SET, LINK, UNION for disjoint sets, and the Kruskal's algorithm, using insertion_sort and disjoint set operations. 
    From the generated graph, we can observe a linear increase in all three algorithms on disjoint sets, where MAKE_SET has the smallest number of operations, followed by 
    UNION, while FIND_SET has significantly more operations, being called four times in total.
*/

#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h";
Profiler pro("Lab9");

typedef struct disjunct
{
    int key;
    disjunct* p;
    int rank;
} disjunct;

typedef struct muchie 
{
    int u, v;
    int cost;
}muchie;

typedef struct graf
{
    int noduri, muchii;
    muchie* m;
    disjunct* x;
}graf;

void MAKE_SET(disjunct* x, Operation& op)
{
    op.count(2);
    x->p = x;
    x->rank = 0;
}

disjunct* FIND_SET(disjunct* x, Operation& op)
{
    op.count();
    if (x != x->p)
        x->p = FIND_SET(x->p, op), op.count();
    return x->p;
}

void LINK(disjunct* x, disjunct* y, Operation& op)
{
    op.count();
    if (x->rank > y->rank)
    {
        y->p = x;
        op.count();
    }
    else
    {
        x->p = y;
        op.count(2);
        if (x->rank == y->rank)
            y->rank++, op.count();
    }
}

void UNION(disjunct* x, disjunct* y, Operation& op1, Operation& op2)
{
    LINK(FIND_SET(x, op1), FIND_SET(y,op1), op2);
}

void insertion_sort(muchie* v, int n)
{
    int i, j;
    muchie aux;
    for (int i = 0; i < n; i++)	//primele i numere vor fi mereu sortate
    {
      aux = v[i];
      j = i - 1;
      while (j >= 0 && v[j].cost > aux.cost)	//shiftez elem mai mari decat aux din partea sortata (dreapta aux)
      {
         v[j + 1] = v[j];
         j--;
      }
      v[j + 1] = aux;		//introduc in pozitia corecta aux
    }
}

muchie* KRUSKAL(graf* g)
{
    Operation total1 = pro.createOperation("MAKE_SET", g->noduri);
    Operation total2 = pro.createOperation("FIND_SET", g->noduri);
    Operation total3 = pro.createOperation("UNION", g->noduri);

    muchie* arbore = (muchie*)malloc((g->noduri - 1) * sizeof(muchie));
    for (int i = 0; i < g->noduri; i++)
        g->x[i].key = i , MAKE_SET(&(g->x[i]), total1);

    insertion_sort(g->m, g->muchii);
    int k = -1;
    for (int i = 0; i < g->muchii; i++)
        if (FIND_SET(&g->x[g->m[i].u], total2) != FIND_SET(&g->x[g->m[i].v], total2))
        {
            arbore[++k] = g->m[i];
            UNION(&g->x[g->m[i].u], &g->x[g->m[i].v], total3, total3);
        }
    return arbore;
}

void demo()
{
    ////////////////////////////////////////////MULTIMI DISJUNCTE////////////////////////////////////////////////
    int n = 13;
    Operation op1 = pro.createOperation("MAKE_SET", n);
    Operation op2 = pro.createOperation("FIND_SET", n);
    Operation op3 = pro.createOperation("UNION", n);
    
    disjunct** x = (disjunct**)malloc((n + 1) * sizeof(disjunct*));
    for (int i = 0; i <= n; i++)
        x[i] = (disjunct*)malloc((n + 1) * sizeof(disjunct));

    for (int i = 1; i <= n; i++)   // 1 2 3 4 5 6 7 8 9 10 11 12 13
    {
        x[i]->key = i;
        MAKE_SET(x[i],op1);
    }
    for (int i = 2; i <= n; i += 2) ///1-2 3-4 5-6 7-8 9-10 11-12
        UNION(x[i - 1], x[i],op2,op3);

    UNION(x[1], x[n], op2, op3); ///1-13-2 3-4 5-6 7-8 9-10 11-12
    UNION(x[1], x[4], op2, op3); ///1-13-2-3-4 5-6 7-8 9-10 11-12

    disjunct* a = (disjunct*)malloc((n + 1) * sizeof(disjunct));
    a = FIND_SET(x[11], op2);
    printf("11 are parinte pe %d\n", a->key);
    a = FIND_SET(x[8], op2);
    printf("8 are parinte pe %d\n", a->key); 
    a = FIND_SET(x[1], op2);
    printf("1 are parinte pe %d\n", a->key); 
    a = FIND_SET(x[4], op2);
    printf("4 are parinte pe %d\n", a->key);
    a = FIND_SET(x[2], op2);
    printf("2 are parinte pe %d\n", a->key);

    //afisare parinti
   /* for (int i = 1; i <= n; i++)
        printf("%d ", x[i]->p->key); */

    //afisare seturi
    printf("\nMultimile disjuncte sunt:\n");
    disjunct* p = (disjunct*)malloc((n + 1) * sizeof(disjunct));
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            p = FIND_SET(x[j], op2);
            if (p->key == x[i]->key)
                printf("%d ", x[j]->key);
        }
        if(x[i]->p->key== i)
        printf("\n");
    }

    ////////////////////////////////////////////KRUSKAL////////////////////////////////////////////////
    int nr_nod = 4, nr_muchii = nr_nod*4;
    graf* g=(graf*) calloc(nr_nod , sizeof(graf));
    g->noduri = nr_nod;
    g->muchii = nr_muchii;
    g->m = (muchie*)calloc(g->muchii, sizeof(muchie));
    g->x = (disjunct*)calloc(g->muchii, sizeof(disjunct));
    g->m[0].u = 0, g->m[0].v = 1, g->m[0].cost = 6;
    g->m[1].u = 0, g->m[1].v = 1, g->m[1].cost = 14;

    g->m[2].u = 0, g->m[2].v = 2, g->m[2].cost = 3;
    g->m[3].u = 0, g->m[3].v = 2, g->m[3].cost = 5;

    g->m[4].u = 0, g->m[4].v = 3, g->m[4].cost = 9;
    g->m[5].u = 0, g->m[5].v = 3, g->m[5].cost = 19;

    g->m[6].u = 1, g->m[6].v = 2, g->m[6].cost = 3;
    g->m[7].u = 1, g->m[7].v = 2, g->m[7].cost = 13;
    g->m[8].u = 1, g->m[8].v = 2, g->m[8].cost = 8;

    g->m[9].u = 1, g->m[9].v = 3, g->m[9].cost = 1;
    g->m[10].u = 1, g->m[10].v = 3, g->m[10].cost = 16;
    g->m[11].u = 1, g->m[11].v = 3, g->m[11].cost = 7;
    g->m[12].u = 1, g->m[12].v = 3, g->m[12].cost = 12;

    g->m[13].u = 2, g->m[13].v = 3, g->m[13].cost = 2;
    g->m[14].u = 2, g->m[14].v = 3, g->m[14].cost = 12;
    g->m[15].u = 2, g->m[15].v = 3, g->m[15].cost = 6;


    printf("\nMuchiile arborelui sunt:\n");
    muchie* arbore= KRUSKAL(g);
    for (int i = 0; i < g->noduri-1; i++)
       printf("%d-%d %d \n", arbore[i].u, arbore[i].v, arbore[i].cost);
}

void perf()
{
    for(int n=100; n<=10000; n+=100)
    {
        graf* g = (graf*)malloc(sizeof(graf));
        g->noduri = n;
        g->muchii = n*4;
        g->m = (muchie*)malloc(g->muchii* sizeof(muchie));
        g->x = (disjunct*)malloc(g->noduri* sizeof(disjunct));

        int* u = (int*)calloc(n, sizeof(int));
        //int* v = (int*)calloc(n, sizeof(int));
        int* cost = (int*)calloc(n, sizeof(int));
        FillRandomArray(u, n, 0, n-1, 1, 0);
        FillRandomArray(cost, n, 1, 100, 0, 0);

        int i, k=0;
        for (i = 0; i < n-1; i++)             //ma asigur ca graful construit iese conex
        {
            g->m[k].u = u[i];
            g->m[k].v = u[i+1];
            g->m[k].cost = cost[i];
            k++;
        }
        g->m[k].u = u[i];
        g->m[k].v = u[0];
        g->m[k].cost = cost[i];
        k++;
        
        for(i=0; i<n; i++)
        {
            int u2 = u[i];
            while(u[i] == u2)
                u2 = rand() % n;
            
            for (int j = 1; j < 4; j++)
            {
                int c = rand() % n +1;
                g->m[k].u = u[i];
                g->m[k].v = u2;
                g->m[k].cost = c;
                k++;
            }
        }

        //for (int i = 0; i < g->muchii; i++)
          //  printf("%d-%d %d \n", g->m[i].u, g->m[i].v, g->m[i].cost);

        //printf("\nMuchiile arborelui sunt:\n");
        muchie* arbore = KRUSKAL(g);
        //for (int i = 0; i < g->noduri - 1; i++)
            //printf("%d-%d %d \n", arbore[i].u, arbore[i].v, arbore[i].cost);

    }
    pro.createGroup("Multimi_disj", "MAKE_SET", "FIND_SET", "UNION");
    pro.showReport();
}

int main()
{
    //demo();
    perf();
    return 0;
}
