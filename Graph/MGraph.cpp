#include <iostream>
#include <cstring>
#include <queue>

using namespace std;

#define MaxVertexNum 100
#define INFINITY 65535

typedef int Vertex; // 用顶点下标表示顶点,为整形
typedef int WeightType; // 边的权值设为int
typedef char DataType;

// 全局变量 Visited[], 初始化为 false
bool Visited[MaxVertexNum];
static void init()
{
    memset(Visited, 0, MaxVertexNum);
}

/* ********************* MGraph *******************/
// Graph struct
typedef struct LGNode
{
    int Nv;
    int Ne;
    WeightType G[MaxVertexNum][MaxVertexNum];
    // DataType Data[MaxVertexNum]; // 存顶点的数据
}* PtrToLGNode;
typedef PtrToLGNode MGraph;

// Edge struct
typedef struct ENode
{
    Vertex V1, V2;
    WeightType Weight;
}* PtrToENode;
typedef PtrToENode Edge;

// 初始化有VertexNum个顶点但没有边的图
MGraph CreateMGraph(int VertexNum){
    MGraph Graph;
    Vertex v,w;

    Graph = (MGraph)malloc(sizeof(struct LGNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    // 初始化邻接矩阵, 并设置所有的边的权值为 INFINITY
    for(v=0; v<VertexNum; v++){
        for(w=0; w<VertexNum; w++){
            Graph->G[v][w] = INFINITY;
        }
    }
    return Graph;
}

// 插入边
void InsertMEdge(MGraph Graph, Edge E){
    // 插入<V1, V2>
    Graph ->G[E->V1][E->V2] = E->Weight;
    //若是无向图,还要插入<V2, V1>
    //Graph->G[E->V2][E->V1] = E->Weight;
}

void DeleteEdge(MGraph Graph, Edge E){
    // 删除<V1, V2>
    Graph ->G[E->V1][E->V2] = INFINITY;
    //若是无向图,还要删除<V2, V1>
    //Graph->G[E->V2][E->V1] = INFINITY;
}

// 建图
MGraph BuildMGraph(){
    MGraph Graph;
    Edge E;
    //Vertex V;
    int Nv, i;

    cin >> Nv;
    Graph = CreateMGraph(Nv);
    cin >> Graph->Ne;

    if (Graph->Ne != 0) //如果边数不为零
    {
        E = (Edge)malloc(sizeof(struct ENode)); // 创建边结点
        for (i=0; i<Graph->Ne; i++)
        {
            cin >> E->V1 >> E->V2 >> E->Weight;
            InsertMEdge(Graph, E);
        }
        free(E);
    }
    /* 如果顶点有数据的话，读入数据
    for (V=0; V<Graph->Nv; V++)
        scanf(" %c", &(Graph->Data[V]));
    */
    return Graph;
}


// 遍历打印图
void PrintMGraph(MGraph G)
{
    for (int i = 0; i < G->Nv; i++)
    {
        for (int j = 0; j < G->Nv; j++){
            if (G->G[i][j] == INFINITY){
                cout << "# ";
                continue;
            }
            cout << G->G[i][j] << " ";
        }
        cout << endl;
    }
}

// 判断是否是邻接矩阵图的边
bool IsMEdge(MGraph Graph, Vertex V, Vertex W)
{
    return Graph->G[V][W] < INFINITY;
}

int Degree(MGraph Graph, Vertex V){
    int ID = 0, OD = 0;
    Vertex W;
    for(W = 0; W < Graph->Nv; W++)
        if(Graph->G[W][V] != INFINITY)
            ID += Graph->G[W][V];
    for(W = 0; W < Graph->Nv; W++)
        if(Graph->G[V][W] != INFINITY)
            OD += Graph->G[V][W];
    return ID + OD;
}

/* ********************* DFS ****************** */
// 邻接矩阵深度优先DFS遍
/* Visited[]为全局变量，已经初始化为false */

void Visit(Vertex V)
{
    cout << "正在访问结点" << V << endl;
}


void DFS(MGraph Graph, Vertex V)
{
    Vertex W;
    Visit(V);
    Visited[V] = true;
    /* 对V的每个邻接点W->AdjV */
    for (W = 0; W < Graph->Nv; W++)  // 对图中的每个顶点遍历
        if (!Visited[W] && IsMEdge(Graph, V, W))
            DFS(Graph, W);
}

void DFSTraverse(MGraph Graph){
    Vertex W;
    init();
    for(W = 0; W < Graph->Nv; W++)
        if(!Visited[W])
            DFS(Graph, W);
}


/* ********************* BFS *******************/
//  邻接矩阵深度优先BFS遍
void BFS(MGraph Graph, Vertex S)
{
    init();
    /* 以S为出发点对邻接矩阵存储的图Graph进行BFS搜索 */
    queue<int>Q;
    Vertex V, W;

    Visit(S);
    Visited[S] = true; /* 标记S已访问 */
    Q.push(S);

    while (!Q.empty()) {
        V = Q.front();
        Q.pop();
        for (W = 0; W < Graph->Nv; W++) /* 对图中的每个顶点W */
            /* 若W是V的邻接点并且未访问过 */
            if (!Visited[W] && IsMEdge(Graph, V, W)) {
                Visit(W);
                Visited[W] = true; /* 标记W已访问 */
                Q.push(W); /* W入队列 */
            }
    }
}

int main(){
    /*---------- 初始测试 ----------*/
    MGraph Graph = BuildMGraph();

    //打印图，用#来表示两个顶点没有关系
    cout << "创建的MGraph为: " << endl;
    PrintMGraph(Graph);
    cout << endl;

    int TD = Degree(Graph, 2);
    cout << "顶点2的度为： " << TD << endl;
    cout << endl;

    Edge E = (Edge)malloc(sizeof(ENode));
    E->V1 = 3;  E->V2 = 4; E->Weight = 8;
    DeleteEdge(Graph, E);
    cout << "删除边<3,4>后的LGraph为： " << endl;
    PrintMGraph(Graph);
    cout << endl;

    cout << "MGraph的DFS遍历: " << endl;
    DFSTraverse(Graph);
    cout << endl;

    cout << "MGraph的BFS遍历: " << endl;
    BFS(Graph, 0);

    return 0;
}

// 测试数据  考虑有向图
/*
5 8
1 0 9
0 2 6
2 4 7
4 3 6
3 1 5
1 2 4
0 3 3
3 4 8
*/

