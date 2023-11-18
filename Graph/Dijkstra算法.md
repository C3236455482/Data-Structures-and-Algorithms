# 最短路径问题



## 有权图的单源最短路算法



### Dijkstra 算法

给定任何一个非负权边的图$v_0,....v_n $，**要找到$v_0$到$v_m$的最短路径**。
设已找到的最短路径的结点集合为$S$ ,未找到最短路径的结点集合为$T$ ,$V_0$到所有结点的最短路径数组为$dist$,初始状态$dist[0] = 0, dist[1..n] = \infty$

**重复以下步骤：**

1. 从集合$T$中基于$dist$中找到当前已知的最短路径结点$v_x$ 加入集合$S$中
2. 基于$v_x$所相连的结点 更新$dist $数组，即$dist\left[ v_y \right] = min(dist\left[ v_y \right], dist\left[ v_x \right] + W(v_x,v_y))$
3. 重复1、2步骤直到所有结点都加入集合$S$



**证明**

设算法迭代每一次加入集合$S$的结点依次为$v_{x_1},v_{x_2} ....v_{x_k}$证明每一次加入$S$中的结点都是最短路径

1. 第一次加入的结点$v_{x_1}$显然是最短路径。
2. 假设第K次加入的结点$v_{x_k}$为最短路径，此时集合S为$\left\{ v_{x_1},v_{x_2},...v_{x_k} \right\}$,现证明根据算法运行第 k + 1次加入的结点$v_{x_{k+1}}$仍为最短路径。
3. 反证，假设加入的$v_{x_{k+1}}$不是最短路径，即存在另外一条路径为最短路径，设$v_u$为该路径中不在集合$S$的第一个结点，则$dist\left[ v_u \right] < dist\left[ v_{x_{k+1}} \right]$，dijkstra的算法的第一个步骤是每次从集合T中基于$dist$选取距离最小的结点加入集合$S$，因此$dist\left[ v_{x_{k+1}} \right] <= dist\left[ v_u \right]$，因此产生矛盾。得证。





路径一定是按照递增（非递减）的顺序生成，且经过收录后经过更新不会改变已收录的点最短路径
如果收录后将$dist$更新得更小，收录点必落在被更新点的路径上，则他必然先被收录

每次从未收录的顶点中选一个dist最小的收录(必为最短路径)==贪心==



代码实现

```cpp
#define ERROR -1
typedef int Vertex; // 用顶点下标表示顶点,为整形


/* ******************************************************************************* */
// Dijkstra 算法
Vertex FindMinDist(MGraph Graph)
{
    Vertex MinV;
    int MinDist = INFINITY;

    for (int V = 0; V < Graph->Nv; V++)
    {
        if (!collected[V] && dist[V] < MinDist)
        {
            // 若 V 未被收录，且 dist[V] 更小
            MinDist = dist[V]; // 更新最小距离
            MinV = V; // 更新对应顶点
        }
    }
    if (MinDist < INFINITY) // 若找到最小 dist
        return MinV; // 返回对应的顶点下标
    else return ERROR; // 若这样的顶点不存在，返回错误标记
}

//无向图的Dijkstra算法
bool Dijkstra(MGraph Graph, Vertex S)
{
    Vertex V, W;

    // 初始化：此处默认邻接矩阵中不存在的边用 INFINITY 表示
    for (V = 0; V < Graph->Nv; V++)
    {
        dist[V] = Graph->G[S][V];
        if (dist[V] < INFINITY)
            path[V] = S; // 更新父节点
        else
            path[V] = -1; // -1 表示不存在父节点
        collected[V] = false; // 初始时将所有点设为未收集状态
    }
    // 先将起点收入集合
    dist[S] = 0;
    collected[S] = true;

    while (1)
    {
        // V = 未被收录顶点中 dist 最小者
        V = FindMinDist(Graph);
        if (V == ERROR) // 若这样的 V 不存在
            break;
        collected[V] = true; // 收录 V
        // 更新新收录的 V 的邻接点
        for (W = 0; W < Graph->Nv; W++)
            // 若 W 是 V 的邻接点并且未被收录
            if (!collected[W] && Graph->G[V][W] < INFINITY)
            {
                if (Graph->G[V][W] < 0) // 若有负边。（实际上，这种情况应该是不存在的，因为我们使用 Dijkstra 算法的前提就是假设无负权边）
                    return false; // 不能正确解决，返回错误标记
                // 若收录 V 使得 dist[W] 变小
                if (dist[V] + Graph->G[V][W] < dist[W])
                {
                    dist[W] = dist[V] + Graph->G[V][W]; // 更新 dist[W]
                    path[W] = V;
                }
            }
    } // while 结束
    return true; // 算法执行结束，返回正确标记
}
```





邻接表+优先队列 实现Dijkstra算法

```cpp
#include<iostream>
#include<vector>
#include<queue>
using namespace std;
#define INFINITY 10000

typedef int Vertex;
struct Edge {
    int Adj;	//邻接点
    int Weight;
};

vector<vector<Edge>> G;
vector<int> dist;
vector<Vertex> path;
int Nv, Ne;

void buildGraph() {
    cin >> Nv >> Ne;
    G.resize(Nv + 1); //调整邻接表的大小为n
    
    for (int i = 0; i < Ne; i++) {
        int v1, v2, w; // v1和v2表示边的两个端点，w表示边的权重
        cin >> v1 >> v2 >> w;
        G[v1].push_back(Edge{ v2, w }); // 在v1的邻接表中添加一条边(v1, v2, w)
        G[v2].push_back(Edge{ v1, w }); // 在v2的邻接表中添加一条边(v2, v1, w)
    }
}
```

```cpp
struct MyType {
    Vertex index;
    int dist;
    friend bool operator < (const MyType& a, const MyType& b) {
        return a.dist > b.dist;	//重载小于号,变成最小堆
    }
};


void dijkstra(Vertex cur) {
    dist = vector<int>(Nv, INFINITY);	//将dist初始化为正无穷,使得以后能够进行更新
    path = vector<Vertex>(Nv, -1);
    vector<bool>collected(Nv, false);

    dist[cur] = 0;

    priority_queue<MyType>H;
    H.push({cur, dist[cur]});

    while (!H.empty()) {
        Vertex V = H.top().index;
        H.pop();
        collected[V] = true;

        for (Vertex W = 0; W < G[V].size(); W++) {
            Vertex next = G[V][W].Adj;	//找到cur的邻接点
            if (!collected[next]) {		//如果当前点未被收录
                if (dist[V] + G[V][W].Weight < dist[next]) {	//且dist可以更新得更小
                    dist[next] = dist[V] + G[V][W].Weight;
                    path[next] = V;
                    H.push({ next,dist[next] });
                }
            }
        }
    }
}
```



注意:

Dijkstra算法不适用负权

Dijkstra算法核心思想是**每次从未确定最短路径的顶点集合中选出距离源点最近的一个顶点，然后以该顶点为中介，更新其他顶点到源点的距离。**
这个过程中，Dijkstra算法会维护一个不变量，即已确定最短路径的顶点集合中的任意顶点到源点的距离都是最短的，而未确定最短路径的顶点集合中的任意顶点到源点的距离都是不小于最短的。

如果图中存在负权边，那么负权边会破坏Dijkstra算法的不变量，使得已确定最短路径的顶点集合中的某些顶点到源点的距离不是最短的，而未确定最短路径的顶点集合中的某些顶点到源点的距离是最短的。这样就会导致Dijkstra算法选错中介顶点，从而得到错误的结果。

