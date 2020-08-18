#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "util.h"
#include "queue.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

struct edge
{
    int src;
    int dst;
    int weight;
};

struct subset
{
    int parent;
    int rank;
};

struct graph
{
    int** adjMat;
    int V;
    int E;
    list_t** adjList;
    struct edge* edges;
};

typedef struct subset subset_t;
typedef struct edge edge_t;
typedef struct graph graph_t;

static edge_t* newEdge(int, int, int);
static void computeAdjList(graph_t*);
static void computeEdges(graph_t*);
static int isValidV(graph_t*, int);
static int minDistance(int*, int*, int);
static void makeSet(subset_t*, int);
static int findSet(subset_t*, int);
static void unionSet(subset_t*, int, int);
static int compareEdge(const void*, const void*);
static int isCyclicRecur(graph_t*, int*, int);
static void DFSRecur(graph_t*, int, int*);
static void topologicalSortRecur(graph_t*, int, int*, list_t*);
static list_t** transpose(graph_t*); 
static void getFinishingTimes(graph_t*, int, int*, list_t*);
static void scc_dfs(graph_t*, int, int*, list_t**);


graph_t* newGraph(void);
void readFromFile(graph_t*, const char*);
void writeToFile(graph_t*, const char*);
void deleteGraph(graph_t**);
int isDirected(graph_t*);
int isAcyclic(graph_t*);
int isDAG(graph_t*);
int adjacent(graph_t*, int, int);
void BFS(graph_t*, int);
void DFS(graph_t*, int, int*);
void Dijkstra(graph_t*, int);
void BellmanFord(graph_t*, int);
void KruskalMST(graph_t*);
void topologicalSort(graph_t*);
void scc(graph_t*);

static edge_t* newEdge(int s, int d, int w)
{
    edge_t* e = (edge_t*)util_calloc(1, sizeof(edge_t));

    e->src = s;
    e->dst = d;
    e->weight = w;

    return e;
}

static void computeAdjList(graph_t* g)
{
    int i,j;
    int** m = g->adjMat;
    int V = g->V;
    int E = 0;

    g->adjList = (list_t**) util_calloc(V, sizeof(list_t*));

    for (i = 0; i < V; ++i)
    {
        g->adjList[i] = newList();

        for (j = 0; j < V; ++j)
        {
            if(m[i][j] != 0)
            {
                edge_t* tmp = newEdge(i, j, m[i][j]);
                insertTail(g->adjList[i], tmp, sizeof(*tmp));
                E++;
                free(tmp);
            }
        }
    }

    util_check(g->E == E, 
               "the adjacency matrix does not match with the adjacency list");
}

static void computeEdges(graph_t* g)
{
    list_t** adjList = g->adjList;
    int i;
    int V = g->V;
    int E = g->E;
    int e = 0;

    edge_t* edges = (edge_t*) util_calloc(E, sizeof(edge_t));

    for (i = 0; i < V; ++i)
    {
        list_iterator_t* it = begin(adjList[i]);
        while(isValidIt(it))
        {
            edge_t* tmp = (edge_t*) derefIt(it);
            edges[e].src = tmp->src;
            edges[e].dst = tmp->dst;
            edges[e].weight = tmp->weight;
            e++;
            nextIt(it);
        }
        deleteIt(it);
    }

    g->edges = edges;
}

static int isValidV(graph_t* g, int v)
{
    return (v > 0 && v < g->V);
}

static int minDistance(int* dist, int* set, int V)
{
    int min = INT_MAX;
    int idx = 0;
    int i;

    for (i = 0; i < V; ++i)
    {
        if(set[i] == 0 && dist[i] < min)
        {
            min = dist[i];
            idx = i;
        }
    }

    return idx;
}

static void makeSet(subset_t* s, int x)
{
    s[x].parent = x;
    s[x].rank = 0;
}

static int findSet(subset_t* s, int x)
{
    if(s[x].parent != x) 
        s[x].parent = findSet(s, s[x].parent);

    return s[x].parent;
}

static void unionSet(subset_t* s, int x, int y)
{
    int xRoot = findSet(s, x);
    int yRoot = findSet(s, y);

    if(xRoot == yRoot) return;

    if(s[xRoot].rank < s[yRoot].rank)
    {
        s[xRoot].parent = yRoot;
    }
    else if(s[xRoot].rank > s[yRoot].rank)
    {
        s[yRoot].parent = xRoot;        
    }
    else
    {
        s[yRoot].parent = xRoot;        
        s[xRoot].rank++;
    }
}

static int compareEdge(const void* x, const void* y)
{
    edge_t* a = (edge_t*) x;
    edge_t* b = (edge_t*) y;

    return a->weight < b->weight;
}

static int isCyclicRecur(graph_t* g, int* visited, int v)
{
    if(visited[v] == WHITE)
    {
        visited[v] = GRAY;

        list_iterator_t* it = begin(g->adjList[v]);

        while(isValidIt(it))
        {
            edge_t* tmp = (edge_t*) derefIt(it);
            if(visited[tmp->dst] == GRAY)
            {
                return 1;
            }

            if(visited[tmp->dst] == WHITE)
            {
                if(isCyclicRecur(g, visited, tmp->dst)) return 1;
            }

            nextIt(it);
        }
        deleteIt(it);

        visited[v] = BLACK;
    }

    return 0;
}

static void DFSRecur(graph_t* g, int v, int* visited)
{
    list_t* adjList = g->adjList[v];

    visited[v] = 1;

    fprintf(stdout, "%d ", v);

    list_iterator_t* it = begin(adjList);

    while(isValidIt(it))
    {
        edge_t* tmp = (edge_t*) derefIt(it);

        if(!visited[tmp->dst])
        {
            DFSRecur(g, tmp->dst, visited);
        }

        nextIt(it);
    }

    deleteIt(it);
}

static void topologicalSortRecur(graph_t* g, int v, int* visited, list_t* list)
{
    visited[v] = GRAY;

    list_iterator_t* it = begin(g->adjList[v]);

    while(isValidIt(it))
    {
        edge_t* tmp = (edge_t*) derefIt(it);
        
        if(visited[tmp->dst] == GRAY)
        {
            fprintf(stderr, "cycle detected: not a DAG!\n");
        }

        if(visited[tmp->dst] == WHITE)
        {
            topologicalSortRecur(g, tmp->dst, visited, list);
        }

        nextIt(it);
    }
    deleteIt(it);

    visited[v] = BLACK;

    insertHead(list, &v, sizeof(v));
}

static list_t** transpose(graph_t* g)
{
    int V = g->V;
    int E = g->E;
    int i;
    edge_t* e = g->edges;

    list_t** list = (list_t**) util_calloc(V ,sizeof(list_t*));

    for(i = 0; i < V; ++i) list[i] = newList();

    for (i = 0; i < E; ++i)
    {
        edge_t* tmp = newEdge(e[i].dst, e[i].src, e[i].weight);
        insertTail(list[tmp->src], tmp, sizeof(*tmp));
        free(tmp);
    }

    return list;
}

static void getFinishingTimes(graph_t* g, int v, int* visited, list_t* list)
{
    visited[v] = GRAY;

    list_iterator_t* it = begin(g->adjList[v]);

    while(isValidIt(it))
    {
        edge_t* tmp = (edge_t*) derefIt(it);

        if(visited[tmp->dst] == WHITE)
        {
            getFinishingTimes(g, tmp->dst, visited, list);
        }

        nextIt(it);
    }
    deleteIt(it);

    visited[v] = BLACK;

    insertHead(list, &v, sizeof(v));
}

static void scc_dfs(graph_t* g, int v, int* visited, list_t** adjListTransposed)
{
    list_t* adjList = adjListTransposed[v];

    visited[v] = 1;

    fprintf(stdout, "%d ", v);

    list_iterator_t* it = begin(adjList);

    while(isValidIt(it))
    {
        edge_t* tmp = (edge_t*) derefIt(it);

        if(!visited[tmp->dst])
        {
            scc_dfs(g, tmp->dst, visited, adjListTransposed);
        }

        nextIt(it);
    }

    deleteIt(it);
}

graph_t* newGraph()
{
    graph_t* g = (graph_t*) util_calloc(1, sizeof(graph_t));

    return g;
}

void readFromFile(graph_t* g, const char* filename)
{
    FILE* fp = util_fopen(filename, "r");

    int V = -1;
    int E = 0;
    int i,j;

    int** m = NULL;

    fscanf(fp, "%d", &V);

    m = (int**)util_calloc(V, sizeof(int*));

    for (i = 0; i < V; ++i)
    {
        m[i] = (int*)util_calloc(V, sizeof(int));

        for (j = 0; j < V; ++j)
        {
            fscanf(fp, "%d", &m[i][j]);
            if(m[i][j] != 0) E++;    
        }
    }

    g->V = V;
    g->E = E;
    g->adjMat = m;

    fclose(fp);

    computeAdjList(g);
    computeEdges(g);
}

void writeToFile(graph_t* g, const char* filename)
{
    FILE* fp = util_fopen(filename, "w");

    int** m = g->adjMat;
    int i,j;
    int V = g->V;

    fprintf(fp, "%d\n", V);

    for (i = 0; i < V; ++i)
    {
        for (j = 0; j < V; ++j)
        {
            fprintf(fp, "%d ", m[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void deleteGraph(graph_t** g)
{
    int i;
    int V = (*g)->V;

    for (i = 0; i < V; ++i)
    {
        free((*g)->adjMat[i]);
    }

    free((*g)->adjMat);

    for (i = 0; i < V; ++i)
    {
        deleteList(&(*g)->adjList[i]);
    }

    free((*g)->adjList);

    free((*g)->edges);

    free(*g);

    *g = NULL;
}

int isDirected(graph_t* g)
{
    int i,j;
    int V = g->V;
    int** m = g->adjMat;

    for (i = 0; i < V-1; ++i)
    {
        for (j = i+1; j < V; ++j)
        {
            if(m[i][j] != m[j][i])
                return 1;
        }
    }

    return 0;
}

int isAcyclic(graph_t* g)
{
    int* visited = (int*)util_calloc(g->V, sizeof(int));

    int i;

    for (i = 0; i < g->V; ++i)
    {
        if(visited[i] == WHITE)
        {
            if(isCyclicRecur(g, visited, i)) return 0;            
        }
    }

    free(visited);

    return 1;
}

int isDAG(graph_t* g)
{
    return isDirected(g) && isAcyclic(g);
}

int adjacent(graph_t* g, int u, int v)
{
    util_check(isValidV(g, u), "vertex u not valid!");
    util_check(isValidV(g, v), "vertex v not valid!");

    if(g->adjMat[u][v] == 0) return 0;

    return 1;
}

void BFS(graph_t* g, int v)
{
    queue_t* q = newQueue();

    int* visited = (int*) util_calloc(g->V, sizeof(int));

    enqueue(q, &v, sizeof(v));
    visited[v] = 1;

    fprintf(stdout, 
            "Breadth first traversal from vertex %d:\n", v);

    while(!isEmpty(q))
    {
        int* tmp = (int*) dequeue(q);

        fprintf(stdout, "%d ", *tmp);

        list_iterator_t* it = begin(g->adjList[*tmp]);

        while(isValidIt(it))
        {
            edge_t* tmp = (edge_t*) derefIt(it);
            
            if(!visited[tmp->dst])
            {
                enqueue(q, &tmp->dst, sizeof(tmp->dst));
                visited[tmp->dst] = 1;
            }

            nextIt(it);
        }
        deleteIt(it);
    }

    deleteQueue(&q);
    free(visited);
}

void DFS(graph_t* g, int v, int* visited)
{
    fprintf(stdout, "Depth first traversal from %d:\n", v);

    DFSRecur(g, v, visited);
}

void Dijkstra(graph_t* g, int v)
{
    int V = g->V;
    int* dist = (int*) util_calloc(V, sizeof(int));
    int* set = (int*) util_calloc(V, sizeof(int));

    int i,j;

    for (i = 0; i < V; ++i) 
        dist[i] = INT_MAX;

    dist[v] = 0;

    for (i = 0; i < V - 1; ++i)
    {
        int u = minDistance(dist, set, V);
        set[u] = 1;

        for (j = 0; j < V; ++j)
        {
            if (!set[j] && g->adjMat[u][j] != 0 &&
                dist[u] + g->adjMat[u][j] < dist[j])
                dist[j] = dist[u] + g->adjMat[u][j];
        }
    }

    fprintf(stdout, "Vertex  Distance\n");
    for (i = 0; i < V; ++i)
    {
        if (dist[i] != INT_MAX)
            fprintf(stdout, "%d\t%d\n", i, dist[i]);
        else
            fprintf(stdout, "%d\tINF", i);
    }

    free(dist);
    free(set);
}

void BellmanFord(graph_t* g, int src)
{
    int V = g->V;
    int E = g->E;
    int* dist = (int*) util_calloc(V, sizeof(int));

    int i,j;

    for (i = 0; i < V; ++i) 
        dist[i] = INT_MAX;

    dist[src] = 0;

    for (i = 0; i <= V - 1; ++i)
    {
        for (j = 0; j < E; ++j)
        {
            int u = g->edges[j].src;
            int v = g->edges[j].dst;
            int w = g->edges[j].weight;

            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
        }
    }

    for (j = 0; j < E; ++j)
    {
        int u = g->edges[j].src;
        int v = g->edges[j].dst;
        int w = g->edges[j].weight;

        if(dist[u] + w < dist[v])
        {
            fprintf(stderr, "negative weight cycle detected\n");
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "Vertex  Distance\n");
    for (i = 0; i < V; ++i)
    {
        if (dist[i] != INT_MAX)
            fprintf(stdout, "%d\t%d\n", i, dist[i]);
        else
            fprintf(stdout, "%d\tINF", i);
    }

    free(dist);
}

void KruskalMST(graph_t* g)
{
    int V = g->V;
    int E = g->E;
    int i;

    list_t* set = newList();
    subset_t* subsets = (subset_t*) util_calloc(V, sizeof(int));

    for(i = 0; i < V; ++i)
        makeSet(subsets, i);

    qsort(g->edges, E, sizeof(g->edges[0]), compareEdge);

    for(i = 0; i < E; ++i)
    {
        int u = g->edges[i].src;
        int v = g->edges[i].dst;

        int x = findSet(subsets, u);
        int y = findSet(subsets, v);

        if(x != y)
        {
            insertTail(set, &g->edges[i], sizeof(g->edges[i]));
            unionSet(subsets, x, y);
        }
    }    

    free(subsets);

    list_iterator_t* it = begin(set);

    while(isValidIt(it))
    {
        edge_t* tmp = (edge_t*) derefIt(it);

        fprintf(stdout, "%d -- %d: %d\n", 
                tmp->src,
                tmp->dst,
                tmp->weight);
        nextIt(it);
    }

    deleteIt(it);

    deleteList(&set);
}

void topologicalSort(graph_t* g)
{
    list_t* list = newList();

    int* visited = (int*) util_calloc(g->V, sizeof(int));

    int i;

    for (i = 0; i < g->V; ++i)
    {
        if(!visited[i])
            topologicalSortRecur(g, i, visited, list);
    }

    fprintf(stdout, "topological sorting:\n");

    list_iterator_t* it = begin(list);

    while(isValidIt(it))
    {
        int* tmp = (int*) derefIt(it);

        fprintf(stdout, "%d ", *tmp);
        nextIt(it);
    }

    deleteIt(it);

    deleteList(&list);
    free(visited);
}

void scc(graph_t* g)
{
    int V = g->V;

    list_t* list = newList();

    int* visited = (int*) util_calloc(V, sizeof(int));

    int i;
    for (i = 0; i < V; ++i)
    {
        if(!visited[i])
            getFinishingTimes(g, i, visited, list);
    }

    list_t** adjListTransposed = transpose(g);

    memset(visited, 0, V * sizeof(int));

    fprintf(stdout, "Strongly connected components:\n");
    
    list_iterator_t* it = begin(list);

    while(isValidIt(it))
    {
        int* tmp = (int*) derefIt(it);

        if(!visited[*tmp])
        {
            scc_dfs(g, *tmp, visited, adjListTransposed);
            fprintf(stdout, "\n");
        }

        nextIt(it);
    }

    deleteIt(it);  

    deleteList(&list);

    free(visited);

    for (i = 0; i < V; ++i)
    {
        deleteList(&adjListTransposed[i]);
    }

    free(adjListTransposed);
}

int main(int argc, char* argv[])
{
    graph_t* g = newGraph();

    readFromFile(g, "input.txt");

    scc(g);

    deleteGraph(&g);

    return 0;
}