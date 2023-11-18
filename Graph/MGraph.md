# “图”（Graph）

表示“多对多”的关系,包含:

- 一组顶点：通常用 V (Vertex) 表示顶点集合
- 一组边：通常用 E (Edge) 表示边的集合
  - 边是顶点对：(v, w) $ \in$ E ，其中 v, w $ \in$ V
  - 有向边 < v, w> 表示从v指向w的边（单行线）
  - 不考虑重边和自回路



## 在程序中表示一个图

邻接矩阵G[N] [N]——N个顶点从0到N-1编号
G[i] [j] = 1 若<vi,vj>是G中的边，否则为INFINITY



### 邻接矩阵

- 直观、简单、好理解
- 方便检查任意一对顶点间是否存在边
- 方便找任一顶点的所有“邻接点”（有边直接相连的顶点）
- 方便计算任一顶点的“度”（从该点发出的边数为“出度”，指向该点的边数为“入度”）
  - 无向图：对应行（或列）非INFINITY元素的个数 
  - 有向图：对应行非INFINITY元素的个数是“出度”；对应列非INFINITY元素的个数是“入度”

浪费空间 —— 存稀疏图（点很多而边很少）有大量无效元素，但对稠密图（特别是完全图）还是很合算的
浪费时间 —— 统计稀疏图中一共有多少条边

> 在稀疏图，或者是需要频繁地找一个点的邻接点的时候，比较适合使用邻接表法



```cpp
#include <iostream>
#include <cstring>
#include <queue>
using namespace std;

#define MaxVertexNum 100
#define INFINITY 65535
typedef int Vertex; // 用顶点下标表示顶点,为整形
typedef int WeightType; // 边的权值设为int
typedef char DataType;
```

**存储结构**

```cpp
// MGraph struct
typedef struct MGNode
{
    int Nv;
    int Ne;
    WeightType G[MaxVertexNum][MaxVertexNum];
    // DataType Data[MaxVertexNum]; // 存顶点的数据
}* PtrToMGNode;
typedef PtrToMGNode MGraph;

// Edge struct
typedef struct ENode
{
    Vertex V1, V2;
    WeightType Weight;
}* PtrToENode;
typedef PtrToENode Edge;
```

**初始化有VertexNum个顶点但没有边的图**

```cpp
MGraph CreateMGraph(int VertexNum){
    MGraph Graph;
    Vertex v,w;

    Graph = (MGraph)malloc(sizeof(struct MGNode));
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
```

**插入边**

```cpp
void InsertMEdge(MGraph Graph, Edge E){
    // 插入<V1, V2>
    Graph ->G[E->V1][E->V2] = E->Weight;
    //若是无向图,还要插入<V2, V1>
    //Graph->G[E->V2][E->V1] = E->Weight;
}
```

**建图**

```cpp
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
```

**用于测试,遍历打印图**

```cpp
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
```



```cpp
// 全局变量 Visited[], 初始化为 false
bool Visited[MaxVertexNum];
static void init()
{
    memset(Visited, 0, MaxVertexNum);
}
void Visit(Vertex V)
{
    cout << "正在访问结点" << V << endl;
}
```

**深度优先搜索DFS**

```cpp
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
```

**广度优先搜索BFS**

```cpp
void BFS(MGraph Graph, Vertex S)
{
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

void BFSTraverse(MGraph Graph){
    Vertex W;
    init();
    for(W = 0; W < Graph->Nv; W++)
        if(!Visited[W])
            BFS(Graph, W);
}
```





### 邻接表

G[N]为指针数组，对应矩阵每行一个链表，只存非0元素

![image-20231117181434581](https://cynthia-picture-for-typora.oss-cn-hangzhou.aliyuncs.com/img_for_typora/image-20231117181434581.png)

邻接表

- 方便找任一顶点的所有“邻接点”
- 节约稀疏图的空间
  - 需要N个头指针 + 2E个结点（每个结点至少2个域）
- 方便计算任一顶点的“度”？
  - 对无向图：是的
  - 对有向图：只能计算“出度”； 否则需要构造“逆邻接表”（存指向自己的边）来方便计算“入度”

不方便检查任意一对顶点间是否存在边！
一定要够稀疏才合算啊~~~~~



```cpp
#include <iostream>
#include <cstring>
#include <queue>
using namespace std;

#define MaxVertexNum 100
#define INFINITY 65535
typedef int Vertex; // 用顶点下标表示顶点,为整形
typedef int WeightType; // 边的权值设为int
typedef char DataType;
```

**存储结构**

```cpp
typedef struct ENode
{
    Vertex V1, V2;
    WeightType Weight;
}*PtrToENode;
typedef PtrToENode Edge;

// 邻接点的定义
typedef struct AdjVNode
{
    Vertex AdjV; // 邻接点下标
    WeightType Weight; // 边权重
    AdjVNode* Next;  // 指向下一个邻接点的致指针
}*PtrToAdjVNode;

// 顶点表头结点的定义 顶点表
struct VNode {
    PtrToAdjVNode FirstEdge;  // 边表头指针
    // DataType Data;   // 存顶点数据,一般不用
};
typedef struct VNode AdjList[MaxVertexNum];

typedef struct LGNode
{
    int Nv; //顶点数
    int Ne; // 边数
    AdjList G; // 邻接表. 是一个size为MaxVertexNum的数组,每一个元素都是一个结点VNode
}*PtrToLGNode;
typedef PtrToLGNode LGraph;
```

**初始化有VertexNum个顶点但没有边的图**

```cpp
LGraph CreateLGraph(int VertexNum)
{
    LGraph Graph;
    Vertex v;

    Graph = (LGraph)malloc(sizeof(struct LGNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    for (v = 0; v < Graph->Nv; v++)
        Graph->G[v].FirstEdge = nullptr; // 每一个顶点v的FirstEdge都是空的, 编号从0到 Graph->Nv-1

    return Graph;
}
```

**插入边**

```cpp
void InsertLEdge(LGraph Graph, Edge E)
{
    auto NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
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
```

**建图**

```cpp
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
        E = (Edge)malloc(sizeof(struct ENode));
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
```

**用于测试,遍历打印图**

```cpp
void PrintLGraph(LGraph Graph)
{
    if (!Graph->G[0].FirstEdge) // 邻接表为空
        return;
    int i;
    for (i = 0; i < Graph->Nv; i++) {
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
```



```cpp
// 全局变量 Visited[], 初始化为 false
bool Visited[MaxVertexNum];
static void init()
{
    memset(Visited, 0, MaxVertexNum);
}
void Visit(Vertex V)
{
    cout << "正在访问结点" << V << endl;
}
```

**深度优先搜索DFS**

```cpp
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

void DFSTraverse(LGraph Graph) {
    Vertex W;
    init();
    for (W = 0; W < Graph->Nv; W++)
        if (!Visited[W])
            DFS(Graph, W);
}
```

**广度优先搜索BFS**

```cpp
void BFS(LGraph Graph, Vertex S)
{   /* 以S为出发点对邻接矩阵存储的图Graph进行BFS搜索 */
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

void BFSTraverse(LGraph Graph) {
    Vertex W;
    init();
    for (W = 0; W < Graph->Nv; W++)
        if (!Visited[W])
            BFS(Graph, W);
}
```





### 简单建立图

当然不可能每次建图都写那么大一长串啦~ 肯定还是用stl来建立图更加方便

```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
#define INFINITY 65535

vector<vector<int>> G;
vector<bool>Visited;  // 全局变量初始化为false
int Nv, Ne;

void buildGraph() {
	int i, v1, v2, w;
	cin >> Nv;
	G = vector<vector<int>>(Nv, vector<int>(Nv, INFINITY));
	Visited = vector<bool>(Nv, false);
	cin >> Ne;
	for (i = 0; i < Ne; i++) {
		cin >> v1 >> v2 >> w;
		G[v1][v2] = w;
		//G[v2][v1] = w;	//若是无向图，还要插入边<V2, V1> 
	}
}
bool IsEdge(int V, int W) {
	return G[V][W] != INFINITY ? true : false;
}
/* ********************* DFS *******************/
/*		DFS(0)		*/
void DFS(int V) {
	Visited[V] = true;
	for (int W = 0; W < Nv; W++)
		if (!Visited[W] && IsEdge(V, W))// W 未被访问并且是 V 的邻接点
			DFS(W);
}

/* ********************* BFS *******************/
/*		BFS(0)		*/
void BFS(int V) {
	int Temp;
	Visited[V] = true;
	queue<int>Q;
	Q.push(V);
	while (!Q.empty()) {
		Temp = Q.front();
		Q.pop();
		for (int W = 0; W < Nv; W++)
			if (!Visited[W] && IsEdge(Temp, W)) {
				Visited[W] = true;
				Q.push(W);
			}
	}
}
```



```cpp
#include<iostream>
#include<vector>
#include<queue>
using namespace std;

struct Edge {
	int Adj;	//邻接点
	int Weight; 
};

vector<vector<Edge>> G;
vector<bool>Visited;
int Nv, Ne;

void buildGraph() {
	cin >> Nv >> Ne; 
	G.resize(Nv); //调整邻接表的大小为n 
	Visited = vector<bool>(Nv, false);
	for (int i = 0; i < Ne; i++) {
		int v1, v2, w; // v1和v2表示边的两个端点，w表示边的权重 
		cin >> v1 >> v2 >> w;
		G[v1].push_back(Edge{ v2, w }); // 在v1的邻接表中添加一条边(v1, v2, w) 
		//G[v2].push_back(Edge{ v1, w }); // 如果是无向图,还需在v2的邻接表中添加一条边(v2, v1, w) 
	} 
}

/* ********************* DFS *******************/
/*		DFS(0)		*/
void DFS(int V) {
	Visited[V] = true;
	for (Vertex W = 0; W < Nv; W++) {
		int next = G[V][W].Adj; 
		if (!Visited[next]) { // 如果邻居节点未被访问
			DFS(next); // 递归地访问邻居节点 
		}
	}
}

/* ********************* BFS *******************/
/*		BFS(0)		*/
void BFS(int V) { 
	queue<int> q;
	q.push(V); 
	Visited[V] = true; 
	while (!q.empty()) {
		V = q.front(); 
		q.pop(); 
		for (int W = 0; W < G[V].size(); W++) {
			int next = G[V][W].Adj; // 获取当前节点的邻居节点 
			if (!Visited[next]) { 
				q.push(next); 
				Visited[next] = true;
			}
		} 
	} 
}
```

