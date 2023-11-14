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


/* ********************* LGraph *******************/
// �ߵĶ���
 typedef struct ENode
 {
     Vertex V1, V2;
     WeightType Weight;
 }* PtrToENode;
typedef PtrToENode Edge;

// �ڽӵ�Ķ���
typedef struct AdjVNode
{
    Vertex AdjV; // �ڽӵ��±�
    WeightType Weight; // ��Ȩ��
    AdjVNode* Next;  // ָ����һ���ڽӵ����ָ��
}* PtrToAdjVNode;

// �����ͷ���Ķ��� �����
struct VNode{
    PtrToAdjVNode FirstEdge;  // �߱�ͷָ��
    // DataType Data;   // �涥������,һ�㲻��
};
typedef struct VNode AdjList[MaxVertexNum];

typedef struct LGNode
{
    int Nv; //������
    int Ne; // ����
    AdjList G; // �ڽӱ�. ��һ��sizeΪMaxVertexNum������,ÿһ��Ԫ�ض���һ�����VNode
}* PtrToLGNode;
typedef PtrToLGNode LGraph;

// ��ʼ����VertexNum�����㵫û�бߵ�ͼ
LGraph CreateLGraph(int VertexNum)
{
    LGraph Graph;
    Vertex v;

    Graph = (LGraph)malloc(sizeof(struct LGNode));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;

    for(v=0; v<Graph->Nv; v++)
        Graph->G[v].FirstEdge = nullptr; // ÿһ������v��FirstEdge���ǿյ�, ��Ŵ�0�� Graph->Nv-1

    return Graph;
}

void InsertLEdge(LGraph Graph, Edge E)
{
    auto NewNode = (PtrToAdjVNode) malloc(sizeof(struct AdjVNode));
    /***************** ����� <V1, V2> ****************/
    /* ΪV2�����µ��ڽӵ� */
    NewNode->AdjV = E->V2;
    NewNode->Weight = E->Weight;
    /* ��V2����V1�ı�ͷ (�����ͷ�Ȳ����β��������ʵ��)*/
    NewNode->Next = Graph->G[E->V1].FirstEdge;
    Graph->G[E->V1].FirstEdge = NewNode;

    /********** ��������ͼ����Ҫ����� <V2, V1> **********/
//    NewNode = (PtrToAdjVNode) malloc(sizeof(struct AdjVNode));
//    /* ΪV1�����µ��ڽӵ� */
//    NewNode->AdjV = E->V1;
//    NewNode->Weight = E->Weight;
//    /* ��V1����V2�ı�ͷ */
//    NewNode->Next = Graph->G[E->V2].FirstEdge;
//    Graph->G[E->V2].FirstEdge = NewNode;
}

// ��ͼ
LGraph BuildLGraph()
{
    LGraph Graph;
    Edge E;
    // Vertex V;
    int Nv, i;

    cin >> Nv;  // ���붥�����
    Graph = CreateLGraph(Nv); // ��ʼ���� Nv �����㵫û�бߵ�ͼ

    cin >> Graph->Ne;   // �������
    if (Graph->Ne != 0)
    { // ����б�
        E = (Edge) malloc(sizeof(struct ENode));
        // �����, ��ʽΪ "���, �յ�, Ȩ��", �����ڽӱ�
        for (i = 0; i < Graph->Ne; i++)
        {
            cin >> E->V1 >> E->V2 >> E->Weight;
            InsertLEdge(Graph, E);
        }
    }

    // ������������ݵĻ�, ��������
    /*for (V = 0; V < Graph->Nv; V++)
    {
        scanf("%c", &(Graph->G[V].Data));
    }*/
    return Graph;
}

// �򵥱���ͼ
void PrintLGraph(LGraph Graph)
{
    if (!Graph->G[0].FirstEdge) // �ڽӱ�Ϊ��
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
// �ڽӱ��������DFS��

void Visit(Vertex V)
{
    printf("���ڷ��ʶ���%d\n", V);
}

void DFS(LGraph Graph, Vertex V)
{
    PtrToAdjVNode W;
    Visit(V);
    Visited[V] = true;
    /* ��V��ÿ���ڽӵ�W->AdjV */
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
// // �ڽӾ����������BFS��
void BFS(LGraph Graph, Vertex S)
{   /* ��SΪ��������ڽӾ���洢��ͼGraph����BFS���� */
    init();
    Vertex V;
    PtrToAdjVNode W;

    queue<int>Q;
    Visit(S);
    Visited[S] = true; /* ���S�ѷ��� */
    Q.push(S);

    while (!Q.empty()) {
        V = Q.front();
        Q.pop();
        for (W = Graph->G[V].FirstEdge; W; W = W->Next) /* ��ͼ�е�ÿ������W */
            /* ��W��V���ڽӵ㲢��δ���ʹ� */
            if (!Visited[W->AdjV]) {
                Visit(W->AdjV);
                Visited[W->AdjV] = true; /* ���W�ѷ��� */
                Q.push(W->AdjV); /* W����� */
            }
    } /* while����*/
}


int main()
{
    LGraph Graph = BuildLGraph();

    cout << "������LGraphΪ: " << endl;
    PrintLGraph(Graph);
    cout << endl;

    cout << "LGraph��DFS����: " << endl;
    DFSTraverse(Graph);
    cout << endl;

    cout << "LGraph��BFS����: " << endl;
    BFS(Graph, 0);


    return 0;
}

// �������� ��������ͼ
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