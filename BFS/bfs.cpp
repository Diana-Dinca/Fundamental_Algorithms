#include <stdlib.h>
#include <string.h>
#include "bfs.h"

int get_neighbors1(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    int k = 0; //number of neighbors
    if((p.col > 0 && p.col <= grid->cols) && (p.row> 0 && p.row<= grid->rows))
        if(grid->mat[p.row][p.col] == 0)
        {
            
            if ((p.row - 1 > 0 && p.row - 1 <= grid->rows))
                if (grid->mat[p.row - 1][p.col] == 0)
                {

                    neighb[k].col = p.col;
                    neighb[k].row = p.row - 1;
                    k++;
                }
            if ((p.row + 1 > 0 && p.row + 1 <= grid->rows))
                if (grid->mat[p.row + 1][p.col] == 0)
                {

                    neighb[k].col = p.col;
                    neighb[k].row = p.row + 1;
                    k++;
                }
            if ((p.col - 1 > 0 && p.col - 1 <= grid->cols))
                if (grid->mat[p.row][p.col - 1] == 0)
                {

                    neighb[k].col = p.col - 1;
                    neighb[k].row = p.row;
                    k++;
                }
            if ((p.col + 1 > 0 && p.col + 1 <= grid->cols))
                if (grid->mat[p.row][p.col + 1] == 0)
                {
                    neighb[k].col = p.col + 1;
                    neighb[k].row = p.row;
                    k++;
                }
            
        }

    return k;
}

//schimb numele in get_neighbors, iar la fct de mai sus get_neighbors1 ca sa nu se apeleze
//schimb dimensiunea vectorului neighb din 4 in 8
int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
    int k = 0; 
    if ((p.col >= 0 && p.col <= grid->cols) && (p.row >= 0 && p.row <= grid->rows))
        if (grid->mat[p.row][p.col] == 0)
        {

            if ((p.row - 2 >= 0 && p.row - 2 <= grid->rows) && (p.col + 1 >= 0 && p.col + 1 <= grid->cols))
                if (grid->mat[p.row - 2][p.col+1] == 0)
                {

                    neighb[k].col = p.col + 1;
                    neighb[k].row = p.row - 2;
                    k++;
                }
            if ((p.row - 2 >= 0 && p.row - 2 <= grid->rows) && (p.col - 1 >= 0 && p.col - 1 <= grid->cols))
                if (grid->mat[p.row - 2][p.col - 1] == 0)
                {

                    neighb[k].col = p.col - 1;
                    neighb[k].row = p.row - 2;
                    k++;
                }
            if ((p.row - 1 >= 0 && p.row - 1 <= grid->rows) && (p.col + 2 >= 0 && p.col + 2 <= grid->cols))
                if (grid->mat[p.row - 1][p.col+2] == 0)
                {

                    neighb[k].col = p.col + 2;
                    neighb[k].row = p.row - 1;
                    k++;
                }
            if ((p.row + 1 >= 0 && p.row + 1 <= grid->rows) && (p.col + 2 >= 0 && p.col + 2 <= grid->cols))
                if (grid->mat[p.row + 1][p.col + 2] == 0)
                {

                    neighb[k].col = p.col + 2;
                    neighb[k].row = p.row + 1;
                    k++;
                }
            if ((p.row + 2 >= 0 && p.row + 2 <= grid->rows) && (p.col + 1 >= 0 && p.col + 1 <= grid->cols))
                if (grid->mat[p.row+2][p.col + 1] == 0)
                {

                    neighb[k].col = p.col + 1;
                    neighb[k].row = p.row + 2;
                    k++;
                }
            if ((p.row + 2 >= 0 && p.row + 2 <= grid->rows) && (p.col - 1 >= 0 && p.col - 1 <= grid->cols))
                if (grid->mat[p.row+2][p.col - 1] == 0)
                {
                    neighb[k].col = p.col - 1;
                    neighb[k].row = p.row + 2;
                    k++;
                }
            if ((p.row + 1 >= 0 && p.row + 1 <= grid->rows) && (p.col + 2 >= 0 && p.col - 2  <= grid->cols))
                if (grid->mat[p.row + 2][p.col + 1] == 0)
                {

                    neighb[k].col = p.col - 2;
                    neighb[k].row = p.row + 1;
                    k++;
                }
            if ((p.row - 1 >= 0 && p.row - 1 <= grid->rows) && (p.col - 2 >= 0 && p.col - 2 <= grid->cols))
                if (grid->mat[p.row - 1][p.col - 2] == 0)
                {
                    neighb[k].col = p.col - 2;
                    neighb[k].row = p.row - 1;
                    k++;
                }
        }

    return k;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[8];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

typedef struct
{
    Node** v;
    int size;
    int capacity;
    int head, tail;
}Queue;

Queue* init(int capacity)
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    q->capacity = capacity;
    q->v = (Node**)malloc(q->capacity * sizeof(Node*));

    return q;
}
void enqueue(Queue* q, Node* x)
{
    if (q->size == q->capacity - 1)
        return;

    q->v[q->tail] = x;
    q->tail++;

    if (q->tail == q->capacity)
        q->tail = 0;

    q->size++;
}

Node* dequeue(Queue* q)
{
    if (q->size == 0)
        return NULL;

    Node* x = q->v[q->head];
    q->head++;

    if (q->head == q->capacity)
        q->head = 0;

    q->size--;
    return x;
}
void bfs(Graph* graph, Node* s, Operation* op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    for (int i = 0; i < graph->nrNodes; i++)
        if(graph->v[i]!=s)
           {
        if (op != NULL) 
            op->count(3);
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = INT_MAX;
        graph->v[i]->parent = NULL;
        }
    if (op != NULL) 
        op->count(3);
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    Queue* q = init(graph->nrNodes);
    enqueue(q, s);

    while (q->size != 0)
    {
        Node* u = dequeue(q);
        for (int i = 0; i < u->adjSize; i++)
        {
            if (op != NULL) 
                op->count();
            if (u->adj[i] != NULL)
            {
                if (op != NULL) 
                    op->count();
                if (u->adj[i]->color == COLOR_WHITE)
                {
                    if (op != NULL) 
                        op->count(3);
                    u->adj[i]->color = COLOR_GRAY;
                    u->adj[i]->dist++;
                    u->adj[i]->parent = u;
                    enqueue(q, u->adj[i]);
                }
            }
        }
        if (op != NULL)
            op->count();
        u->color = COLOR_BLACK;
    }
}


void pretty_print(int* v, int n,Point *repr, int root, int lvl)
{
    for (int i = 0; i < lvl; i++)
        printf(" ");

    printf("(%d, %d)\n", repr[root].row, repr[root].col);

    for (int i = 0; i < n; i++)
        if (v[i] == root)
            pretty_print(v, n,repr, i, lvl + 2);

}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        for (int i = 0; i < graph->nrNodes; i++)
            if (p[i] == -1)
                pretty_print(p, n, repr, i, 0);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    bfs(graph, start, NULL);
    Node* nod = end;
    int k = 0;
    Node** aux = (Node**) calloc(1000, sizeof(Node*));
    aux[k++] = end;
    while(nod!= start && nod!= NULL)
    {
        nod = nod->parent;
        aux[k++] = nod;
    }
    if (nod == NULL)
        return - 1;
    for (int i = 0; i < k; i++)
        path[i] = aux[k- 1 - i];
    return k ;
}


void performance() {
    int n, i;
    Profiler p("bfs");
    srand(time(nullptr));

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected

        
        for (int i = 0; i < graph.nrNodes; i++) {
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));
            graph.v[i]->adjSize = 0;
            graph.v[i]->adj[graph.v[i]->adjSize++] = graph.v[i + 1];
        }
        graph.v[graph.nrNodes-1]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        graph.v[graph.nrNodes-1]->adjSize = 0;
        for (int i= graph.nrNodes-1;i < n; i++)
        {
            int src = rand() % graph.nrNodes;
            int dest = rand() % graph.nrNodes;

            while (src == dest)
            { 
                src = rand() % graph.nrNodes;
                dest = rand() % graph.nrNodes;
            }
            int ok = 0;
            for (int j = 0; j < graph.v[src]->adjSize; j++)
                if (graph.v[src]->adj[j] == graph.v[dest])
                    ok = 1;
            if(ok==0)
            {
                graph.v[src]->adj[graph.v[src]->adjSize++] = graph.v[dest];
               
            }
            if (ok == 1)
                i--;
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        for (int i = 0; i < n - 1; i++) 
        {
            graph.v[i]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));
            graph.v[i]->adjSize = 0;
            graph.v[i]->adj[graph.v[i]->adjSize++] = graph.v[i + 1];
        }
        graph.v[n - 1]->adj = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        graph.v[n - 1]->adjSize = 0;
        for(int i= graph.nrNodes-1; i < 4500; i++)
        {
            int src = rand() % graph.nrNodes;
            int dest = rand() % graph.nrNodes;

            while (src == dest)
            {
                src = rand() % graph.nrNodes;
                dest = rand() % graph.nrNodes;
            }
            int ok = 0;
            for (int j = 0; j < graph.v[src]->adjSize; j++)
                if (graph.v[src]->adj[j] == graph.v[dest])
                    ok = 1;
            if (ok == 0)
            {
                graph.v[src]->adj[graph.v[src]->adjSize++] = graph.v[dest];
            }
            if (ok == 1)
                i--;
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}