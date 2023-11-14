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


/* ********************* LGraph *******************/
// 边的定义
 typedef struct ENode
 {
     Vertex V1, V2;
     WeightType Weight;
 }* PtrToENode;
typedef PtrToENode Edge;

// 邻接点的定义
typedef struct AdjVNode
{
    Vertex AdjV; // 邻接点下标
    WeightType Weight; // 边权重
    AdjVNode* Next;  // 指向下一个邻接点的致指针
}* PtrToAdjVNode;

// 顶点表头结点的定义 顶点表
struct VNode{
    PtrToAdjVNode FirstEdge;  // 边表头指针
    // DataType Data;   // 存顶点数据,一般不用
};
typedef struct VNode AdjList[MaxVertexNum];

typedef struct LGNode
{
    int Nv; //顶点数
    int Ne; // 边数
    AdjList G; // 邻接表. 是一个size为MaxVertexNum的数组,每一个元素都是一个结点VNode
}* PtrToLGNode;
typedef PtrToLGNode LGraph;

// 初始化有VertexNum个顶点但没有边的图
LGraph CreateLGraph(int VertexNum)
{
    LGraph Graph;
    Vertex v;

    Graph = (LGraph)malloc(sizeof(struct LGNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    for(v=0; v<Graph->Nv; v++)
        Graph->G[v].FirstEdge = nullptr; // 每一个顶点v的FirstEdge都是空的, 编号从0到 Graph->Nv-1

    return Graph;
}

void InsertLEdge(LGraph Graph, Edge E)
{
    auto NewNode = (PtrToAdjVNode) malloc(sizeof(struct AdjVNode));
    /***************** 插入边 <V1, V2> ****************/
    /* 为V2建立新的邻接点 */
    NewNode->AdjV = E->V2;
    NewNode->Weight = E->Weight;
    /* 将V2插入V1的表头 (插入表头比插入表尾明显容易实现)*/
    NewNode->Next = Graph->G[E->V1].FirstEdge;
    Graph->G[E->V1].FirstEdge = NewNode;

    /********** 若是无向图，还要插入边 <V2, V1> **********/
//    NewNode = (PtrToAdjVNode) malloc(sizeof(struct AdjVNode));
//    /* 为V1建立新的邻接点 */
//    NewNode->AdjV = E->V1;
//    NewNode->Weight = E->Weight;
//    /* 将V1插入V2的表头 */
//    NewNode->Next = Graph->G[E->V2].FirstEdge;
//    Graph->G[E->V2].FirstEdge = NewNode;
}

// 建图
LGraph BuildLGraph()
{
    LGraph Graph;
    Edge E;
    // Vertex V;
    int Nv, i;

    cin >> Nv;  // 读入顶点个数
    Graph = CreateLGraph(Nv); // 初始化有 Nv 个顶点但没有边的图

    cin >> Graph->Ne;   // 读入边数
    if (Graph->Ne != 0)
    { // 如果有边
        E = (Edge) malloc(sizeof(struct ENode));
        // 读入边, 格式为 "起点, 终点, 权重", 插入邻接表
        for (i = 0; i < Graph->Ne; i++)
        {
            cin >> E->V1 >> E->V2 >> E->Weight;
            InsertLEdge(Graph, E);
        }
    }

    // 如果顶点有数据的话, 读入数据
    /*for (V = 0; V < Graph->Nv; V++)
    {
        scanf("%c", &(Graph->G[V].Data));
    }*/
    return Graph;
}

// 简单遍历图
void PrintLGraph(LGraph Graph)
{
    if (!Graph->G[0].FirstEdge) // 邻接表为空
        return;
    int i;
    for (i = 0; i < Graph->Nv; i++){
        cout << i << ": | ";

        PtrToAdjVNode tmp = Graph->G[i].FirstEdge;
        while (tmp)
        {
            cout << tmp->AdjV << " " << tmp->Weight << " | ";
            tmp = tmp->Next;
        }
        cout << endl;
    }
}

/* ********************* DFS *******************/
// 邻接表深度优先DFS遍

void Visit(Vertex V)
{
    printf("正在访问顶点%d\n", V);
}

void DFS(LGraph Graph, Vertex V)
{
    PtrToAdjVNode W;
    Visit(V);
    Visited[V] = true;
    /* 对V的每个邻接点W->AdjV */
    for (W = Graph->G[V].FirstEdge; W; W = W->Next)
        if (!Visited[W->AdjV])
            DFS(Graph, W->AdjV);
}

void DFSTraverse(LGraph Graph){
    Vertex W;
    init();
    for(W = 0; W < Graph->Nv; W++)
        if(!Visited[W])
            DFS(Graph, W);
}

/* ********************* BFS *******************/
// // 邻接矩阵深度优先BFS遍
void BFS(LGraph Graph, Vertex S)
{   /* 以S为出发点对邻接矩阵存储的图Graph进行BFS搜索 */
    init();
    Vertex V;
    PtrToAdjVNode W;

    queue<int>Q;
    Visit(S);
    Visited[S] = true; /* 标记S已访问 */
    Q.push(S);

    while (!Q.empty()) {
        V = Q.front();
        Q.pop();
        for (W = Graph->G[V].FirstEdge; W; W = W->Next) /* 对图中的每个顶点W */
            /* 若W是V的邻接点并且未访问过 */
            if (!Visited[W->AdjV]) {
                Visit(W->AdjV);
                Visited[W->AdjV] = true; /* 标记W已访问 */
                Q.push(W->AdjV); /* W入队列 */
            }
    } /* while结束*/
}


int main()
{
    LGraph Graph = BuildLGraph();

    cout << "创建的LGraph为: " << endl;
    PrintLGraph(Graph);
    cout << endl;

    cout << "LGraph的DFS遍历: " << endl;
    DFSTraverse(Graph);
    cout << endl;

    cout << "LGraph的BFS遍历: " << endl;
    BFS(Graph, 0);


    return 0;
}

// 测试数据 考虑有向图
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