/*
    I implemented functions for insertion and search in a hash table using open addressing and quadratic probing, on a data structure called Entry.
    I defined N=10007 and calculated, for each load factor {0.8, 0.85, 0.9, 0.95, 0.99}, the operations performed in the process of searching for both found and 
    unfound elements in the hash table.
    I observed that the average effort for found elements is lower than the average effort for unfound elements in the table, as well as in the case of the maximum 
    effort for a found element compared to unfound ones.
    I also implemented an algorithm that counts the search operations after deleting the elements from the load factor 0.99 down to 0.8, and I noticed that the 
    resulting values are close to the initial ones for the 0.8 load factor.

*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include "Profiler.h"

#define N 10007
#define nr_tests 5
#define m 3000

Profiler pro("Hash");

FILE* pf = fopen("Tabel.csv", "w");

typedef struct {
    int id;
    char name[30];
} Entry;

int hashing(int key, int i, int n)
{
    int k1 = 3, k2 = 5, k3 = 7;
    return (key + k1 * i * i + k2 * i + k3) % n;
}

int hash_insert(Entry* v, int n, int key, const char name[])
{
    int i = 0;
    while (i < n)
    {
        int j = hashing(key, i, n);
        if (v[j].id == -1)
        {
            strcpy_s(v[j].name, name);
            v[j].id = key;
            return 0;
        }
        i++;
    }
    return -1;
}

int hash_search(Entry* v, int n, int key, const char name[], int* op)
{
    int i = 0;
    (*op)++;
    int j = hashing(key, i, n);
    while (i < n && v[j].id != key && v[j].id != -1 && strcmp(v[j].name, "deleted") != 0)
    {
        (*op)++;
        i++;
        j = hashing(key, i, n);
    }
    if (i < n && v[j].id != -1)
    {
        //if(strcmp(v[j].name, name) == 0)
        return j;
    }
    return -1;
}

void stergere(Entry* v, int n, int key, const char name[])
{
    int op;
    int poz = hash_search(v, n, key, name, &op);
    v[poz].id = -1;
    strcpy(v[poz].name, "deleted");
}

void factor_umplere(float factor, int* op, int* opn)
{

    Entry* v = (Entry*)calloc(N, sizeof(Entry));
    for (int i = 0; i < N; i++)
        v[i].id = -1;

    float factor2 = 0;
    float size = 0;

    int* a = (int*)calloc(N, sizeof(int));        //vector cu N elemente generate random
    int* m_vec = (int*)calloc(1500, sizeof(int)); //vector cu 1500 de elemente regasite in tabel
    int* m_vec2 = (int*)calloc(1500, sizeof(int)); //vector cu 1500 de elemente NEregasite in tabel
    int* index = (int*)calloc(1500, sizeof(int)); //vector cu 1500 de pozitii generate random

    float nrMaxFound = 0, nrMaxNotFound = 0;
    float totalFound = 0, totalNotFound = 0;

    for (int j = 0; j < nr_tests; j++)
    {
        while (factor2 < factor)
        {
            FillRandomArray(a, N, 0, 100000, 1, 0);
            int i = 0;
            while (i < N && factor2 < factor)
            {
                if (hash_insert(v, N, a[i], "name") != -1)
                    size++;
                else
                    a[i] = -1;
                i++;
                factor2 = size / N;
            }
        }
        ///elemente regasite
        FillRandomArray(index, 1500, 0, N - 1, 1, 0); //generez random 1500 de poziti 
        for (int q = 0; q < 1500; q++)
            for (int k = 0; k < N; k++)
            {
                if (a[index[q]] != -1)    //adaug in vector daca gasesc din prima 
                    m_vec[q] = a[index[q]];
                else
                {
                    int w = index[q];
                    while (a[w] != -1)   //caut primul element !=-1 
                        w++;
                    m_vec[q] = a[index[w]];
                }
            }
        ///elemente NEregasite
        FillRandomArray(m_vec2, 1500, 100000, 200000, 1, 0);

        //cautare
        for (int k = 0; k < 1500; k++)
        {
            (*op) = 0;
            (*opn) = 0;
            hash_search(v, N, m_vec[k], "name", op);
            totalFound += (*op);
            if (nrMaxFound < *op)
                nrMaxFound = (*op);

            hash_search(v, N, m_vec2[k], "name", opn);
            totalNotFound += (*opn);
            if (nrMaxNotFound < *opn)
                nrMaxNotFound = (*opn);
        }
    }
    totalFound /= nr_tests;
    totalNotFound /= nr_tests;
    totalFound /= 1500;
    totalNotFound /= 1500;
    fprintf(pf, "%f, %f, %f, %f, %f\n", factor, totalFound, nrMaxFound, totalNotFound, nrMaxNotFound);

    ////////////////////////////////////////////////stergere///////////////////////////////////////////////////
    if (factor >= 0.99)
    {
        //fprintf(pf, "Factor de umplere dupa stergere, Efort mediu gasite dupa stergere, Efort maxim gasite dupa stergere, Efort mediu negasite, Efort maxim negasite\n");

        while (factor > 0.8)
        {
            for (int i = 0; i < 1500; i++)
            {
                stergere(v, N, m_vec[i], "name");
                int sop = 0;
                int poz = hash_search(v, N, m_vec[i], "name", &sop);
                a[poz] = -1;

                size--;
                factor = size / N;
                if (factor <= 0.8)
                    break;
                
            }
            ///elemente regasite
            FillRandomArray(index, 1500, 0, N - 1, 1, 0); //generez random 1500 de poziti 
            for (int q = 0; q < 1500; q++)
                for (int k = 0; k < N; k++)
                {
                    if (a[index[q]] != -1)    //adaug in vector daca gasesc din prima 
                        m_vec[q] = a[index[q]];
                    else
                    {
                        int w = index[q];
                        while (a[w] != -1)   //caut primul element !=-1 
                            w++;
                        m_vec[q] = a[index[w]];
                    }
                }
        }

        ///elemente regasite
        FillRandomArray(index, 1500, 0, N - 1, 1, 0); //generez random 1500 de poziti 
        for (int q = 0; q < 1500; q++)
            for (int k = 0; k < N; k++)
            {
                if (a[index[q]] != -1)    //adaug in vector daca gasesc din prima 
                    m_vec[q] = a[index[q]];
                else
                {
                    int w = index[q];
                    while (a[w] != -1)   //caut primul element !=-1 
                        w++;
                    m_vec[q] = a[index[w]];
                }
            }
        ///elemente NEregasite
        FillRandomArray(m_vec2, 1500, 100000, 200000, 1, 0);

        //cautare
        totalFound = 0;
        totalNotFound = 0;
        nrMaxFound = 0;
        nrMaxNotFound = 0;
        for (int k = 0; k < 1500; k++)
        {
            int(sop) = 0;
            int(sopn) = 0;
            hash_search(v, N, m_vec[k], "name", &sop);
            totalFound += (sop);
            if (nrMaxFound < sop)
                nrMaxFound = (sop);

            hash_search(v, N, m_vec2[k], "name", &sopn);
            totalNotFound += (sopn);
            if (nrMaxNotFound < sopn)
                nrMaxNotFound = (sopn);
        }
        totalFound /= 1500;
        totalNotFound /= 1500;
        fprintf(pf, "%f, %f, %f, %f, %f\n", factor, totalFound, nrMaxFound, totalNotFound, nrMaxNotFound);

    }


    //for (int i = 0; i < 1500; i++)
      // printf("%d \n", m_vec[i]);

    /*for (int i = 0; i < N; i++)
    {
        if (v[i].id == -1)
            printf("NULL\n");
        else
            printf("id: %d   nume: %s \n", v[i].id, v[i].name);
    }*/

}

void alfa()
{

    fprintf(pf, "Factor de umplere, Efort mediu gasite, Efort maxim gasite, Efort mediu negasite, Efort maxim negasite\n");

    int op1 = 0, op1n = 0;
    factor_umplere(0.8, &op1, &op1n);

    int op2 = 0, op2n = 0;
    factor_umplere(0.85, &op2, &op2n);

    int op3 = 0, op3n = 0;
    factor_umplere(0.9, &op3, &op3n);

    int op4 = 0, op4n = 0;
    factor_umplere(0.95, &op4, &op4n);

    int op5 = 0, op5n = 0;
    factor_umplere(0.99, &op5, &op5n);
}

void demo()
{
    int n = 13;
    Entry* v = (Entry*)calloc(n, sizeof(Entry));
    for (int i = 0; i < n; i++)
        v[i].id = -1;

    hash_insert(v, n, 1, "Ana");
    hash_insert(v, n, 2, "Briana");
    hash_insert(v, n, 3, "Calin");
    hash_insert(v, n, 4, "Dana");
    hash_insert(v, n, 5, "Ana");
    hash_insert(v, n, 6, "Briana");
    hash_insert(v, n, 7, "Emanuel");
    hash_insert(v, n, 8, "Daniel");

    printf("  Hash initial:\n");
    for (int i = 0; i < n; i++)
        printf("id: %d, name: %s \n", v[i].id, v[i].name);

    stergere(v, n, 7, "Emanuel");
    stergere(v, n, 5, "Ana");

    printf("\n\n\n Hash dupa stergere:\n");
    for (int i = 0; i < n; i++)
        printf("id: %d, name: %s \n", v[i].id, v[i].name);

    int op = 0;
    printf("Cheia %d se afla pe pozitia %d \n", 1, hash_search(v, n, 1, "Ana", &op));
    printf("Cheia %d se afla pe pozitia %d \n", 5, hash_search(v, n, 5, "Aana", &op));
    printf("Cheia %d se afla pe pozitia %d \n", 7, hash_search(v, n, 7, "Emanuel", &op));
    printf("Cheia %d se afla pe pozitia %d \n", 8, hash_search(v, n, 8, "Daniel", &op));
}


int main()
{
    demo();
    //alfa();

    /*Operation op = pro.createOperation("Alfa", N);
    factor_umplere(0.8, op);*/
    return 0;
}