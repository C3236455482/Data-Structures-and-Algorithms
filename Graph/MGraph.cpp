#include <iostream>
#include <cstring>
#include <queue>

using namespace std;

#define MaxVertexNum 100
#define INFINITY 65535

typedef int Vertex; // �ö����±��ʾ����,Ϊ����
typedef int WeightType; // �ߵ�Ȩֵ��Ϊint
typedef char DataType;

// ȫ�ֱ��� Visited[], ��ʼ��Ϊ false
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
    // DataType Data[MaxVertexNum]; // �涥�������
}* PtrToLGNode;
typedef PtrToLGNode MGraph;

// Edge struct
typedef struct ENode
{
    Vertex V1, V2;
    WeightType Weight;
}* PtrToENode;
typedef PtrToENode Edge;

// ��ʼ����VertexNum�����㵫û�бߵ�ͼ
MGraph CreateMGraph(int VertexNum){
    MGraph Graph;
    Vertex v,w;

    Graph = (MGraph)malloc(sizeof(struct LGNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    // ��ʼ���ڽӾ���, ���������еıߵ�ȨֵΪ INFINITY
    for(v=0; v<VertexNum; v++){
        for(w=0; w<VertexNum; w++){
            Graph->G[v][w] = INFINITY;
        }
    }
    return Graph;
}

// �����
void InsertMEdge(MGraph Graph, Edge E){
    // ����<V1, V2>
    Graph ->G[E->V1][E->V2] = E->Weight;
    //��������ͼ,��Ҫ����<V2, V1>
    //Graph->G[E->V2][E->V1] = E->Weight;
}

void DeleteEdge(MGraph Graph, Edge E){
    // ɾ��<V1, V2>
    Graph ->G[E->V1][E->V2] = INFINITY;
    //��������ͼ,��Ҫɾ��<V2, V1>
    //Graph->G[E->V2][E->V1] = INFINITY;
}

// ��ͼ
MGraph BuildMGraph(){
    MGraph Graph;
    Edge E;
    //Vertex V;
    int Nv, i;

    cin >> Nv;
    Graph = CreateMGraph(Nv);
    cin >> Graph->Ne;

    if (Graph->Ne != 0) //���������Ϊ��
    {
        E = (Edge)malloc(sizeof(struct ENode)); // �����߽��
        for (i=0; i<Graph->Ne; i++)
        {
            cin >> E->V1 >> E->V2 >> E->Weight;
            InsertMEdge(Graph, E);
        }
        free(E);
    }
    /* ������������ݵĻ�����������
    for (V=0; V<Graph->Nv; V++)
        scanf(" %c", &(Graph->Data[V]));
    */
    return Graph;
}


// ������ӡͼ
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

// �ж��Ƿ����ڽӾ���ͼ�ı�
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
// �ڽӾ����������DFS��
/* Visited[]Ϊȫ�ֱ������Ѿ���ʼ��Ϊfalse */

void Visit(Vertex V)
{
    cout << "���ڷ��ʽ��" << V << endl;
}


void DFS(MGraph Graph, Vertex V)
{
    Vertex W;
    Visit(V);
    Visited[V] = true;
    /* ��V��ÿ���ڽӵ�W->AdjV */
    for (W = 0; W < Graph->Nv; W++)  // ��ͼ�е�ÿ���������
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
//  �ڽӾ����������BFS��
void BFS(MGraph Graph, Vertex S)
{
    init();
    /* ��SΪ��������ڽӾ���洢��ͼGraph����BFS���� */
    queue<int>Q;
    Vertex V, W;

    Visit(S);
    Visited[S] = true; /* ���S�ѷ��� */
    Q.push(S);

    while (!Q.empty()) {
        V = Q.front();
        Q.pop();
        for (W = 0; W < Graph->Nv; W++) /* ��ͼ�е�ÿ������W */
            /* ��W��V���ڽӵ㲢��δ���ʹ� */
            if (!Visited[W] && IsMEdge(Graph, V, W)) {
                Visit(W);
                Visited[W] = true; /* ���W�ѷ��� */
                Q.push(W); /* W����� */
            }
    }
}

int main(){
    /*---------- ��ʼ���� ----------*/
    MGraph Graph = BuildMGraph();

    //��ӡͼ����#����ʾ��������û�й�ϵ
    cout << "������MGraphΪ: " << endl;
    PrintMGraph(Graph);
    cout << endl;

    int TD = Degree(Graph, 2);
    cout << "����2�Ķ�Ϊ�� " << TD << endl;
    cout << endl;

    Edge E = (Edge)malloc(sizeof(ENode));
    E->V1 = 3;  E->V2 = 4; E->Weight = 8;
    DeleteEdge(Graph, E);
    cout << "ɾ����<3,4>���LGraphΪ�� " << endl;
    PrintMGraph(Graph);
    cout << endl;

    cout << "MGraph��DFS����: " << endl;
    DFSTraverse(Graph);
    cout << endl;

    cout << "MGraph��BFS����: " << endl;
    BFS(Graph, 0);

    return 0;
}

// ��������  ��������ͼ
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

