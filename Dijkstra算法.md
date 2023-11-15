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

