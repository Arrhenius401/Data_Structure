#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>


#define MAXSIZE 100
#define MINSIZE -114514


//心得:为包含了指向另一个结构体变量的指针的结构体变量使用malloc函数后
//需要指针再次使用malloc来分配空间

//辅助队列
typedef struct{
    int num;
    struct QSNode *next;
}QSNode;

typedef struct {
    QSNode *front,*rear;
}GraphQueue;



//建造图书元素
typedef struct
{
    /* data */
    int uid;
    char name[20];
    float price;
}Book;

typedef Book ElemType;
typedef int WeightType;
typedef int VexName;

//定义边
typedef struct
{
    VexName v1,v2;          //表示两个节点,同时<v1,v2>是有向边
    WeightType weight   //权重
}Edge;

//定义边结点
typedef struct{
    int theNex;        //该边所指表头结点序号
    WeightType weight;  //边自身的权重
    struct Anode *next; //下一条边的指针
}Anode;

//定义表头结点
typedef struct{
    ElemType *elem;
    Anode *firstEdge;
}Vnode;

//定义包含边与结点及对应数量的图
typedef struct{
    int vexnum,arcnum;
    Vnode *Vnodes[MAXSIZE];
}ALGraph;

bool visited[MAXSIZE];  //访问标志数组



//辅助队列功能
//判断队列是否为空
bool isQueueEmpty(GraphQueue *q){
    if(q->front == q->rear){
        return true;
    }
    return false;
}

//1,初始化
GraphQueue *initGQ(){
    GraphQueue *gq = (GraphQueue*)malloc(sizeof(GraphQueue));
    QSNode *head = (QSNode*)malloc(sizeof(QSNode));
    head->num = -1;
    head->next = NULL;
    gq->front = gq->rear = head;
    return gq;
}

//2,入队列
void enQueue(GraphQueue *q,int v){
    QSNode *n = (QSNode*)malloc(sizeof(QSNode));
    n->num = v;
    n->next = NULL;

    q->rear->next = n;
    q->rear = n;
}

//3,出队列
int deQueue(GraphQueue *q){
    if(isQueueEmpty(q)){
        printf("队列为空\n");
        return -1;
    }
    QSNode *s;
    s = q->front;     //这是头结点head
    s = s->next;
    int point = s->num;

    q->front->next = s->next;
    if(q->rear == s){
        //最后一个节点被删时
        q->rear = q->front;
    }
    free(s);
    return point;
}


//正文内容
//1,初始化
ALGraph *initG(int vnum){
    //生成有vnum个顶点但是没有边的图
    ALGraph *g = (ALGraph*)malloc(sizeof(ALGraph));
    g->arcnum = 0;
    g->vexnum = vnum;

    for(int v=0;v < g->vexnum;v++){
        g->Vnodes[v] = (Vnode*)malloc(sizeof(Vnode));
        g->Vnodes[v]->firstEdge = NULL;
    }
    return g;
}

//2,添加结点
void addVex(ALGraph *g,ElemType e,int VID){
    g->Vnodes[VID]->elem = (ElemType*)malloc(sizeof(ElemType));
    *(g->Vnodes[VID]->elem) = e;
}

//3,添加边
void addArc(ALGraph *g,Edge e,bool isDirected){
    //将edge除v1外的基础数据填入新建的边结点里
    Anode *newNode = (Anode*)malloc(sizeof(Anode));
    newNode->theNex = e.v2;
    newNode->weight = e.weight;
    //插入表头
    newNode->next = g->Vnodes[e.v1]->firstEdge;
    g->Vnodes[e.v1]->firstEdge = newNode;

    g->arcnum++;

    //考虑无向图
    if(!isDirected){
        //重新分配一地址,否则下面的操作将覆盖上面原有的数据
        newNode = (Anode*)malloc(sizeof(Anode));
        newNode->theNex = e.v1;
        newNode->weight = e.weight;
        newNode->next = g->Vnodes[e.v2]->firstEdge;
        g->Vnodes[e.v2]->firstEdge = newNode;
    }
}

//4.0,初始化访问标志数组
void initVisited(int vnum){
    for(int v=0;v<vnum;v++){
        visited[v] = false;
    }
}

//4.1,深度优先搜索
void AL_DFS(ALGraph *g,int v){
    //从图中某个顶点v出发,访问v,并置visited[v]的值为true
    ElemType *e = g->Vnodes[v]->elem;
    printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);
    visited[v] = true;
    //依次检查v的所有邻接点
    Anode *an = g->Vnodes[v]->firstEdge;
    while(an){
        if(!visited[an->theNex]){
            AL_DFS(g,an->theNex);
        }
        an = an->next;
        
    }
}

//4.2,广度优先搜索
void AL_BFS(ALGraph *g,int v){
    GraphQueue *q = initGQ();
    enQueue(q,v);
    visited[v] = true;
    while(!isQueueEmpty(q)){
        //每一次循环,打印队列第一个元素信息
        int vnow = deQueue(q);
        ElemType *e = g->Vnodes[vnow]->elem;
        printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);
        for(Anode *an = g->Vnodes[vnow]->firstEdge;an != NULL;an = an->next){
            if(!visited[an->theNex]){
                enQueue(q,an->theNex);
                visited[an->theNex] = true;
            }
        } 
    }
}

//打印图
void LookAll(ALGraph *g){
    for(int i=0;i < g->vexnum;i++){
        printf("结点%d:",i+1);
        Vnode *vn = g->Vnodes[i];
        ElemType *e = vn->elem;
        printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);
        Anode *an = vn->firstEdge;
        while (an)
        {
            printf("存在权重为%d的邻接边,并与图书%d相连\n",an->weight,an->theNex+1);
            an = an->next;
        }
        
    }
}



//5.1,拓扑排序AOV-网
int topo[MAXSIZE];

bool TopoLogicalSort(ALGraph *g,int topo[]){
    int n = g->vexnum;
    //初始化算法所需的辅助数据结构indegree[MAXSIXE][n],queue
    int indegree[MAXSIZE];      //存放各个结点的入度
    GraphQueue *queue = initGQ();

    for(int v=0;v<n;v++){
        indegree[v] = 0;
    }
    for(int v=0;v<n;v++){
        for(Anode *an=g->Vnodes[v]->firstEdge;an !=NULL;an = an->next){
            indegree[an->theNex]++;
        }
    }
    //将所有入度为0的结点入列
    for(int v=0;v<n;v++){
        if(!indegree[v]){
            enQueue(queue,v);
        }
    }
    //进入拓扑排序
    int cut = 0;    //记录topo[]顺序
    while (!isQueueEmpty(queue))
    {
        int empty = deQueue(queue);
        topo[cut++] = empty;
        for(Anode *an=g->Vnodes[empty]->firstEdge;an != NULL;an=an->next){
            if(--indegree[an->theNex]==0){
                enQueue(queue,an->theNex);
            }
        }
    }
    //若存在回路
    if(cut < g->vexnum){
        return false;
    }
    return true;
}

//5.2,拓扑排序AOE-网(查找关键路径)
bool TopoCrticalPath(ALGraph *g,int topu[]){
    //调用拓扑排序算法
    if(!TopoLogicalSort(g,topo)){
        return false;
    }
    int n = g->vexnum;
    //引入辅助数据结构
    int ve[n],vl[n];    //每个事件的最早发生时间和最晚发生时间
    for(int v=0;v<n;v++){
        ve[v] = 0;
    }
/*----------按拓扑次序求每个事件的最早发生时间----------*/
    for(int v=0;v<n;v++){
        int theN = topo[v];
        Anode *an = g->Vnodes[theN]->firstEdge;
        while (an!=NULL){
            //依次更新theN的所有邻接点的最早发生时间(挑最晚的)
            int adjN = an->theNex;
            if(ve[adjN] < ve[theN]+an->weight){
                ve[adjN] = ve[theN]+an->weight;
            }
            an = an->next;
        }
    }
/*----------逆拓扑次序求每个事件的最晚发生时间----------*/
    //初始化vl,初值为ve[n-1]
    for(int v=0;v<n;v++){
        vl[v] = ve[n-1];
    }
    for(int v=n-1;v>=0;v--){
        int theN = topo[v];
        Anode *an = g->Vnodes[theN]->firstEdge;
        while (an!=NULL){
            //依次更新theN的所有邻接点的最晚发生时间(挑最早的)
            int adjN = an->theNex;
            if(vl[theN] > vl[adjN]-an->weight){
                vl[theN] = vl[adjN]-an->weight;
            }
            an = an->next;
        }
    }
/*------------判断每一项活动是否是关键活动------------*/
    //可以选择将关键活动打印下来
    printf("存在关键活动:");
    for(int v=0;v<n;v++){
        Anode *an = g->Vnodes[v]->firstEdge;
        while (an!=NULL){
            int adjN = an->theNex;
            int earlytime = ve[v];
            int latetime = vl[adjN]-an->weight;
            if(earlytime == latetime){
                printf(" <%d,%d>",v,adjN);
            }
            an = an->next;
        }      
    }

    return true;
}




//最小生成树
//Kruskal算法的辅助数据结构,并查集
typedef struct{
    int *parent;    //记录各个结点的父结点
    int *rank;      //记录每棵树的秩
    int count;      //记录连通分支的数量
}UnionFind;

//6.1.0.1.1,并查集的初始化
UnionFind *initUF(int n){
    UnionFind *uf = (UnionFind*)malloc(sizeof(UnionFind));
    uf->parent = (int*)malloc(sizeof(int)*n);
    uf->rank = (int*)malloc(sizeof(int)*n);
    uf->count = n;

    for(int i=0;i<n;i++){
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
    
    return uf;
}

//6.1.0.1.2,并查集的查找操作
int findset(UnionFind *uf,int theN){
    if(theN!=uf->parent[theN]){
        //顺带路径压缩
        uf->parent[theN] = findset(uf,uf->parent[theN]);
    }
    return uf->parent[theN];
}

//6.1.0.1.3,并查集的合并操作
void unionset(UnionFind *uf,int x,int y){
    int rootX = findset(uf,x);
    int rootY = findset(uf,y);
    if(rootX!=rootY){
        if(uf->rank[x] > uf->rank[y]){
            uf->parent[y] = rootX;
        }else if(uf->rank[x] < uf->rank[y]){
            uf->parent[x] = rootY;
        }else{
            uf->parent[y] = rootX;
            uf->rank[x]++;
        }
    }
    uf->count--;
}


//Kruskal算法的辅助数据结构,最小堆
typedef struct 
{
    Edge *edge;     //堆的数据域
    int size;       //堆目前的大小
    int capacity;    //堆最大的大小
}MinHeap;

//6.1.0.2.2,堆的插入
bool insertMH(MinHeap *h,Edge e){
    //做可行性分析
    if(h->size == (h->capacity+1)){
        return false;
    }
    //i指向插入堆中的最后一个元素的后一个位置
    //通过迭代i最终指向应该插入的位置,同时让其他元素给目标元素让位
    int i = ++h->size;
    for(;h->edge[i/2].weight > e.weight;i=i/2){
        h->edge[i] = h->edge[i/2];
    }
    h->edge[i] = e;

    return true;
}

//6.1.0.2.1,堆的初始化
MinHeap *initMH(ALGraph *g){
    int n = g->arcnum;
    //设置堆的指针的常规数据
    MinHeap *h = (MinHeap*)malloc(sizeof(MinHeap));
    h->capacity = n;
    h->size = 0;
    h->edge = (Edge*)malloc(sizeof(Edge)*(n+1));
    //设置哨兵
    h->edge[0].weight = MINSIZE;

    for(int v=0;v<g->vexnum;v++){
        for(Anode *an=g->Vnodes[v]->firstEdge;an!=NULL;an=an->next){
            if(an->theNex > v){
                //只收v1<v2的边,以避免重复录入无向图的边
                Edge e;
                e.v1 = v;
                e.v2 = an->theNex;
                e.weight = an->weight;
                insertMH(h,e);
            }
        }
    }

    return h;
}


//6.1.0.2.3,堆的吐出
Edge *popMH(MinHeap *h){
    //做可行性分析
    if(h->size == 0){
        return NULL;
    }
    //取出最小边
    Edge *minEdge = (Edge*)malloc(sizeof(Edge));
    *minEdge = h->edge[1];
    //删除结点
    Edge lastEdge = h->edge[h->size];
    int parent,child;
    for(parent=1;parent*2 <= h->size;parent = child){
        //找到父结点中权值较小的那个结点
        child = parent*2;
        if(h->edge[child].weight > h->edge[child+1].weight){
            child++;
        }
        if(lastEdge.weight < h->edge[parent].weight){
            //将堆最后一个元素安置在父结点的位置
            h->edge[parent] = lastEdge;
            break;
        }else{
            //将子结点安置在父结点的位置
            h->edge[parent] = h->edge[child];
        }
    }
    h->size--;

    return minEdge;
}

//6.1.0.2.4,遍历堆
void LookHeap(MinHeap *h){
    printf("操作后最小堆为:");
    for(int i=0;i<h->size;i++){
        printf("<%d,%d>权值为%d   ",h->edge[i+1].v1+1,h->edge[i+1].v2+1,h->edge[i+1].weight);
    }
    printf("\n");
}
/*-------------------------------辅助数据结构准备就绪-------------------------------------*/
//6.1.0.3,判断是否构成回路
bool isNoCycle(UnionFind *uf,int x,int y){
    int rootX = findset(uf,x);
    int rootY = findset(uf,y);
    if(rootX == rootY){
        //不能要
        return false;
    }else{
        //可以要,同时将两结点并入同一连通集
        unionset(uf,x,y);
        return true;
    }
}

//6.1.0.4,打印MST
void LookMST(int n,Edge e_MST[n]){
    Edge e;
    printf("存在MST:\n");
    for(int i=0;i<n;i++){
        e = e_MST[i];
        printf("<%d,%d>,权重为%d\n",e.v1+1,e.v2+1,e.weight);
    }
}

//6.1.1,Kruskal算法
void KruskalMST(ALGraph *g){
    int n = g->vexnum;
    //引入辅助数据结构
    Edge e_MST[n-1];    //存储构成MST的边
    //初始化并查集和最小堆
    UnionFind *uf = initUF(n);
    MinHeap *hp = initMH(g);

    int count = 0;
    while(count<n-1){
        Edge *e = popMH(hp);
        if(isNoCycle(uf,e->v1,e->v2)){
            e_MST[count]=*e;
            count++;
        }
    }

    //打印获得的数据
    LookMST(n-1,e_MST);
}



int main(int argc, char const *argv[])
{
    //准备图书数据
    Book book[9];
    book[0].uid = 21;
    book[1].uid = 18;
    book[2].uid = 35;
    book[3].uid = 24;
    book[4].uid = 45;
    book[5].uid = 26;
    book[0].price = 12;
    book[1].price = 16;
    book[2].price = 17;
    book[3].price = 111;
    book[4].price = 342;
    book[5].price = 54;
    strcpy(book[0].name,"图书1");
    strcpy(book[1].name,"图书2");
    strcpy(book[2].name,"图书3");
    strcpy(book[3].name,"图书4");
    strcpy(book[4].name,"图书5");
    strcpy(book[5].name,"图书6");

    //准备边数据
    Edge edge[6];
    edge[0].v1 = 0;
    edge[1].v1 = 0;
    edge[2].v1 = 0;
    edge[3].v1 = 1;
    edge[4].v1 = 3;
    edge[5].v1 = 5;

    edge[0].v2 = 1;
    edge[1].v2 = 2;
    edge[2].v2 = 5;
    edge[3].v2 = 3;
    edge[4].v2 = 4;
    edge[5].v2 = 3;
    edge[0].weight = 9;
    edge[1].weight = 3;
    edge[2].weight = 7;
    edge[3].weight = 10;
    edge[4].weight = 6;
    edge[5].weight = 8;

    //检验初始化
    ALGraph *g = initG(6);

    //检验添加功能
     for(int v=0;v<6;v++){
        addVex(g,book[v],v);
    }
    printf("检验有向图情况\n");
    for(int a=0;a<6;a++){
        addArc(g,edge[a],true);
    }
    LookAll(g);
    printf("***************************\n");

    g = initG(6);
    for(int v=0;v<6;v++){
        addVex(g,book[v],v);
    }
    printf("检验无向图情况\n");
    for(int a=0;a<6;a++){
        addArc(g,edge[a],false);
    }
    LookAll(g);
    printf("***************************\n");


    

    //检验DFS
    printf("检验DFS\n");
    initVisited(6);
    AL_DFS(g,1);
    printf("***************************\n");

    //检验BFS
    printf("检验BFS\n");
    initVisited(6);
    AL_BFS(g,1);
    printf("***************************\n");
    

    
    //为拓扑排序准备新的数据
    book[6].uid = 48;
    book[6].price = 16;
    strcpy(book[6].name,"事件7");

    book[7].uid = 46;
    book[7].price = 74;
    strcpy(book[7].name,"事件8");

    book[8].uid = 99;
    book[8].price = 56;
    strcpy(book[8].name,"事件9");

    strcpy(book[0].name,"事件1");
    strcpy(book[1].name,"事件2");
    strcpy(book[2].name,"事件3");
    strcpy(book[3].name,"事件4");
    strcpy(book[4].name,"事件5");
    strcpy(book[5].name,"事件6");

    Edge newEdge[11];
    newEdge[0].v1 = 0;
    newEdge[1].v1 = 0;
    newEdge[2].v1 = 0;
    newEdge[3].v1 = 1;
    newEdge[4].v1 = 2;
    newEdge[5].v1 = 3;
    newEdge[6].v1 = 4;
    newEdge[7].v1 = 4;
    newEdge[8].v1 = 5;
    newEdge[9].v1 = 6;
    newEdge[10].v1 = 7;


    newEdge[0].v2 = 1;
    newEdge[1].v2 = 2;
    newEdge[2].v2 = 3;
    newEdge[3].v2 = 4;
    newEdge[4].v2 = 4;
    newEdge[5].v2 = 5;
    newEdge[6].v2 = 6;
    newEdge[7].v2 = 7;
    newEdge[8].v2 = 7;
    newEdge[9].v2 = 8;
    newEdge[10].v2 = 8;

    newEdge[0].weight = 6;
    newEdge[1].weight = 4;
    newEdge[2].weight = 5;
    newEdge[3].weight = 1;
    newEdge[4].weight = 1;
    newEdge[5].weight = 2;
    newEdge[6].weight = 9;
    newEdge[7].weight = 7;
    newEdge[8].weight = 4;
    newEdge[9].weight = 2;
    newEdge[10].weight = 4;

    g = initG(9);

    for(int i=0;i<7;i++){
        addVex(g,book[i],i);
    }

    for(int i=0;i<11;i++){
        addArc(g,newEdge[i],true);
    }

    //检验AOV-网的拓扑排序
    if(TopoLogicalSort(g,topo)){
        printf("该图拓扑排序为:");
        for(int v=0;v<g->vexnum;v++){
            printf(" %d",topo[v]);
        }
        printf("\n");
        printf("***************************\n");
    }

    //检验AOE-网的拓扑排序--查找关键路径
    TopoCrticalPath(g,topo);
    printf("\n");
    printf("***************************\n");

    //为Kruskal算法准备新数据
    newEdge[0].v1 = 2;
    newEdge[1].v1 = 2;
    newEdge[2].v1 = 2;
    newEdge[3].v1 = 2;
    newEdge[4].v1 = 2;
    newEdge[5].v1 = 0;
    newEdge[6].v1 = 0;
    newEdge[7].v1 = 4;
    newEdge[8].v1 = 4;
    newEdge[9].v1 = 5;

    newEdge[0].v2 = 0;
    newEdge[1].v2 = 1;
    newEdge[2].v2 = 3;
    newEdge[3].v2 = 4;
    newEdge[4].v2 = 5;
    newEdge[5].v2 = 1;
    newEdge[6].v2 = 3;
    newEdge[7].v2 = 1;
    newEdge[8].v2 = 5;
    newEdge[9].v2 = 3;

    newEdge[0].weight = 1;
    newEdge[1].weight = 5;
    newEdge[2].weight = 5;
    newEdge[3].weight = 6;
    newEdge[4].weight = 4;
    newEdge[5].weight = 6;
    newEdge[6].weight = 5;
    newEdge[7].weight = 3;
    newEdge[8].weight = 6;
    newEdge[9].weight = 2;

    g = initG(6);

    for(int i=0;i<6;i++){
        addVex(g,book[i],i);
    }

    for(int i=0;i<10;i++){
        addArc(g,newEdge[i],false);
    }

    //检验生成MST的Kruskal算法
    KruskalMST(g);
    printf("***************************\n");

    return 0;
}
