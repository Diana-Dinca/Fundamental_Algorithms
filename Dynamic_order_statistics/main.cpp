/*
    I implemented a function to construct a perfectly balanced binary tree, achieved in O(N), a function to select the i-th smallest term from the tree, achieved in O(logN), 
    and a deletion function, also achieved in O(logN). The functionality of these algorithms is highlighted using a pretty print function.
    On the graph generated for the management of operations, it can be observed that although BUILD_TREE has a higher complexity, it performs fewer operations than selection 
    and deletion. This is because these two functions are called for n terms, while construction is called only once.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

Profiler pro("BinaryTree");
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

typedef struct node
{
    int key;
    int size;
    node* left;
    node* right;
    node* parent;
} node;

node* BUILD_TREE(int l, int r, node* father, int n)
{
    if (l > r) return NULL;
    Operation opT = pro.createOperation("BUILD_TREE", n);

    int mid = (l + r) / 2;      //mijlocul intervalului de chei
    node* root = (node*)malloc(sizeof(node));
    opT.count(5);
    root->key = mid;
    root->size = 1;
    root->parent = father;

    root->left = BUILD_TREE(l, mid - 1, root, n);  //mid - 1 = nr de elem din stanga mid, k = cheia de start
    root->right = BUILD_TREE(mid + 1, r, root, n);  //n - mid = nr de elem din dreapta mid, k + mid = noua cheie de start

    opT.count();
    if (root->left != NULL)
    {
        opT.count();
        root->size += root->left->size;
    }
    opT.count();
    if (root->right != NULL)
    {
        opT.count();
        root->size += root->right->size;
    }
    return root;
}

node* OS_SELECT(node* root, int i, int n)
{
    int r = 1;
    Operation opT = pro.createOperation("OS_SELECT", n);
    opT.count();
    if (root != NULL)
    {
        opT.count();
        if (root->left != NULL)
            r = root->left->size + 1;

        if (i == r)
            return root;
        else if (i < r)
            return OS_SELECT(root->left, i, n);
        else
            return OS_SELECT(root->right, i - r, n);
    }
    return NULL;
}

node* findMax(node* root, int n)
{
    Operation opT = pro.createOperation("OS_DELETE", n);
    opT.count();
    while (root->right != NULL)
        root = root->right, opT.count(2);

    return root;
}

void resize(node* root, int n)
{
    Operation opT = pro.createOperation("OS_DELETE", n);
    opT.count();
    if (root->parent != NULL)
    {
        opT.count();
        root->parent->size--;
        resize(root->parent, n);
    }
    return;
}

void OS_DELETE(node* r, int i, int n)
{
    Operation opT = pro.createOperation("OS_DELETE", n);
    opT.count();
    if (r != NULL)
    {
        opT.count(2);
        if (i < r->key)
            OS_DELETE(r->left, i, n);
        else if (i > r->key)
            OS_DELETE(r->right, i, n);
        else
        {
            opT.count(6);
            if (r->left == NULL && r->right != NULL) //are decat copil dreapta
            {
                opT.count(2);
                r->key = r->right->key;
                resize(r->right, n);
                //free(r->right);
                r->right = NULL;
            }
            else if (r->left != NULL && r->right == NULL) //are decat copil stanga
            {
                opT.count(2);
                r->key = r->left->key;
                resize(r->left, n);
                //free(r->left);
                r->left = NULL;
            }
            else if (r->left != NULL && r->right != NULL) //are ambii copii
            {
                opT.count(2);
                node* before = findMax(r->left, n);
                r->key = before->key;
                if (before->parent->right->key == r->key)
                    before->parent->right = NULL, opT.count();
                else
                    before->parent->left = NULL, opT.count();
                resize(before, n);
               // free(before);
            }
            else  //nu are copii
            {
                opT.count();
                if (r->parent != NULL)
                {
                    if (r->parent->right != NULL && r->parent->right->key == i)
                        r->parent->right = NULL, opT.count();
                    else if (r->parent->left != NULL && r->parent->left->key == i)
                        r->parent->left = NULL, opT.count();
                    resize(r, n);
                }
                //free(r);

            }

        }
    }
}

void printBinaryTree(node* root, int level)
{
    if (root != NULL)
    {
        for (int i = 0; i < level; i++)
            printf("  ");

        printf("%d(%d)\n", root->key, root->size);
        printBinaryTree(root->left, level + 1);
        printBinaryTree(root->right, level + 1);
    }
}

void demo()
{
    int n = 11;
    node* root = BUILD_TREE(1, n, NULL, n);
    printf("Radacina este: %d\n", root->key);
    printf("Arborele binar echilibrat este: \n");
    printBinaryTree(root,0);
    printf("\n");

    int cheie = 3;
    node* root2 = OS_SELECT(root, cheie, n);
    printf("A %d-a cea mai mica cheie este: %d\n", cheie, root2->key);
    printf("\n\n");

    OS_DELETE(root, 2, n);
    printBinaryTree(root, 0);
    printf("\n\n");

    OS_DELETE(root, 9, n);
    printBinaryTree(root, 0);
    printf("\n\n");

    OS_DELETE(root, 3, n);
    printBinaryTree(root, 0);
    printf("\n\n");

    OS_DELETE(root, 6, n);
    printBinaryTree(root, 0);

}

void perf()
{
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
        for (int i = 1; i <= NR_TESTS; i++)
        {
            node* root = BUILD_TREE(1, n, NULL, n);
            int nn = n;

            for (int j = 1; j <= n; j++)
            {
                int k = rand() % nn + 1;            //al k-lea cel mai mic
                nn--;
                node* root2 = OS_SELECT(root, k, n);
                if(root2!=NULL)
                    OS_DELETE(root, root2->key, n);
            }
        }
    pro.divideValues("BUILD_TREE", NR_TESTS);
    pro.divideValues("OS_SELECT", NR_TESTS);
    pro.divideValues("OS_DELETE", NR_TESTS);

    pro.createGroup("Dynamic_Order_Statistics", "BUILD_TREE", "OS_DELETE", "OS_SELECT");
    pro.showReport();

}

int main()
{
    demo();
    //perf();
    return 0;
}
