#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>

#define MAXSIZE 100
#define MAXINT 999



//辅助队列
typedef struct{
    int num;
    struct node *next;
}node;

typedef struct 
{
    node *front,*rear;
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

typedef struct{
    int vexnum,arcnum;
    ElemType vexs[MAXSIZE];
    WeightType arcs[MAXSIZE][MAXSIZE];
}AMGraph;

typedef struct
{
    int v1,v2;          //表示两个节点,同时<v1,v2>是有向边
    WeightType weight;   //权重
}Edge;

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
    node *head = (node*)malloc(sizeof(node));
    head->num = -1;
    head->next = NULL;
    gq->front = gq->rear = head;
    return gq;
}

//2,入队列
void enQueue(GraphQueue *q,int v){
    node *n = (node*)malloc(sizeof(node));
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
    node *s;
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



//1,初始化
AMGraph *initG(int vnum){
    AMGraph *g;

    g = (AMGraph*)malloc(sizeof(AMGraph));
    g->vexnum = vnum;
    g->arcnum = 0;

    for(int V=0;V<g->vexnum;V++){
        for(int W=0;W<g->vexnum;W++){
            g->arcs[V][W] = MAXINT; 
        }
    }
    return g;
}

//2,添加边
void addArc(AMGraph *g,Edge e,bool isDirected){
    g->arcs[e.v1][e.v2] = e.weight;
    g->arcnum++;
    if(!isDirected){
        g->arcs[e.v2][e.v1] = e.weight;
    }
}

//3,添加结点
void addVex(AMGraph *g,ElemType e,int v){
    g->vexs[v] = e;
}

//图的遍历
//4.0,初始化访问标志数组
void initVisited(int vnum){
    for(int v=0;v<vnum;v++){
        visited[v] = false;
    }
}

//4.1,深度优先搜索
void AM_DFS(AMGraph *g,int v){
    //从图中某个顶点v出发,访问v,并置visited[v]的值为true
    ElemType e = g->vexs[v];
    printf("uid=%d,name=%s,price=%f\n",e.uid,e.name,e.price);
    visited[v] = true;
    //依次检查v的所有邻接点
    for(int w=0;w <  g->vexnum;w++){
        if(g->arcs[v][w]<MAXINT&&!visited[w]){
            AM_DFS(g,w);
        }
    }
}

//4.2,广度优先搜索
void AM_BFS(AMGraph *g,int v){
    GraphQueue *q = initGQ();
    //进入队列即认定结点已被访问
    enQueue(q,v);
    visited[v] = true;
    while (!isQueueEmpty(q))
    {
        //每一次循环,打印队列第一个元素信息
        int vnow = deQueue(q);
        ElemType e = g->vexs[vnow];
        printf("uid=%d,name=%s,price=%f\n",e.uid,e.name,e.price);
        //找到目前结点的所有邻接点
        for(int w=0;w < g->vexnum;w++){
            if(g->arcs[vnow][w]<MAXINT&&!visited[w]){
                enQueue(q,w);
                visited[w] = true;
            }
        }
    }  
}

//查找最短路径
//5.0.1,打印由Dijkstra算法获得的最短路径
void LookShortestPath_Dij(int v0,int n,bool isShortest[n],int distance[n],int path[n]){
    for(int v=0;v<n;v++){
        printf("%d结点到%d结点\n",v0+1,v+1);
        if(isShortest[v]){
            printf("    ");
            printf("存在长度为%d的最短路径\n",distance[v]);
            printf("    ");
            printf("具体路径为");
            int theN = v;
            while (theN!=v0)
            {
                printf(" %d",theN+1);
                theN = path[theN];
            }
            printf(" %d\n",v0+1);
            
        }else{
            printf("    ");
            printf("不连通\n");
        }
    }
}

//5.0.2,打印由Floyd算法获得的最短路径
void LookShortestPath_Floyd(int n,int distance[n][n],int path[n][n]){
    for(int v=0;v<n;v++){
        printf("对于结点%d\n",v+1);
        for(int w=0;w<n;w++){
            printf("    ");
            if(distance[v][w]==MAXINT){
                printf("到结点%d最短路径不存在\n",w+1);
            }else{
                printf("到结点%d最短路径为%d\n",w+1,distance[v][w]);
                printf("    ");
                printf("具体为");
                printf(" %d",w+1);
                int i = path[v][w];
                while (i!=-1)
                {
                    printf(" %d",i+1);
                    i = path[v][i];
                }
                printf("\n");      
            }
            printf("--------------------------\n");
        }
        printf("结点%d结束\n",v+1);
        printf("----------------------------------------------\n");
    }
}


//5.1,Dijkstra算法
void ShortestPath_Dij(AMGraph *g,int v0){
    //初始化算法所需的辅助数据结构isShortest[n],distance[n],path[n]
    int n = g->vexnum;
    bool isShortest[n];     //记录从源点v0到终点vi是否已被确定最短路径长度
    int distance[n];        //记录从源点v0到终点vi的当前最短路径长度  
    int path[n];            //记录从源点v0到终点vi的当前最短路径上vi的直接前驱结点序号
    for(int v=0;v<n;v++){
        isShortest[v] = false;
        distance[v] = g->arcs[v0][v];   //将v0到各个终点的最短路径长度初始化为弧上的权值
        if(distance[v]<MAXINT){
            path[v] = 0;    //若v0与v之间有弧,则将v的前驱设为v0
        }else{
            path[v] = -1;   //若v0与v之间无弧,则将v的前驱设为-1
        }
    }
    isShortest[v0] = true;
    distance[v0] = 0;
/*-----------初始化结束,进入主循环,每次求得v0到某个顶点的最短路径,并更新辅助数据结构--------------*/
    for(int i=1;i<n;i++){   //对剩余n-1个结点进行操作
        int minD = MAXINT;
        int minN = 0;
        for(int w=0;w<n;w++){   //找到与theN结点邻接的最短边及对应结点
            if(!isShortest[w]&&distance[w]<minD){
                minD = distance[w];
                minN = w;
            }
        }
        //获取数据后,更新三个辅助数据结构数组
        isShortest[minN] = true;
        for(int w=0;w<n;w++){
            if(!isShortest[w]&&(distance[minN]+g->arcs[minN][w] < distance[w])){
                distance[w] = distance[minN]+g->arcs[minN][w];
                path[w] = minN;
            }
        }
    }
/*------------------------------------输出获得的数据---------------------------------------*/
    LookShortestPath_Dij(v0,n,isShortest,distance,path);
}

//5.2,Floyd算法
void ShortestPath_Floyd(AMGraph *g){
    //初始化算法所需的辅助数据结构distance[n][n],path[n][n]
    int n = g->vexnum;
    int distance[n][n];     //记录顶点vi和vj之间的最短路径长度
    int path[n][n];         //最短路径上顶点vj的前一顶点的序号
    for(int v=0;v<n;v++){
        for(int w=0;w<n;w++){
            distance[v][w] = g->arcs[v][w];
            if(distance[v][w]<MAXINT&&v!=w){
                path[v][w] = v;
            }else{
                path[v][w] = -1;
            }
        }
    }
/*-----------初始化结束,进入主循环,每次求得v0到某个顶点的最短路径,并更新辅助数据结构--------------*/
    for(int k=0;k<n;k++){
        for(int v=0;v<n;v++){
            for(int w=0;w<n;w++){
                if(distance[v][k]+distance[k][w]<distance[v][w] && v!=w){
                    distance[v][w] = distance[v][k] + distance[k][w];
                    path[v][w] = path[k][w];
                }
            }
        }
    }
    //打印数据
    LookShortestPath_Floyd(n,distance,path);
}

//最小生成树
//6.1.0.1,Prim算法的辅助函数
//找到集合U-V中,与集合U任意结点邻接边的权重值最小的结点
int minKey(int n,int key[],bool isMST[]){
    int minW,minN;
    minW = MAXINT;

    for(int v=0;v<n;v++){
        if(!isMST[v]&&key[v]<minW){
            minW = key[v];
            minN = v;
        }
    }
    return minN;
}

//6.1.0.2,打印通过Prim算法得到的MST
void LookMST(int n,int parent[]){
    printf("最小生成树为:");
    //我的MST从0开始
    for(int v=1;v<n;v++){
        printf(" <%d,%d>",v+1,parent[v]+1);
    }
    printf("\n");
}

//6.1.1,Prim算法
void PrimMST(AMGraph *g){
    int n = g->vexnum;
    //引入辅助数据结构
    int partent[n];     //保存所有节点的父节点,即parent[x]是x结点的父结点(它不是并查集)
    int key[n];         //保存"目前"所有节点的权重最小的边
    bool isMST[n];    //表示结点是否在MST集合里
    //初始化辅助数据结构
    for(int v=0;v<n;v++){
        key[v] = MAXINT;
        isMST[v] = false;
    }

    //从第一个结点开始,直接取为0
    key[0] = 0;
    partent[0] = -1;

    //循环n-1次,取n-1条边
    for(int count=0;count<n;count++){
        //添加U-V中符合要求的结点
        int k = minKey(n,key,isMST);
        //更新三个辅助数据结构
        isMST[k] = true;
        for(int v=0;v<n;v++){
            if(g->arcs[k][v]<key[v]&&!isMST[v]){
                partent[v] = k;
                key[v] = g->arcs[k][v];
            }
        }
    }
    //打印获得的数据
    LookMST(n,partent);
}

//打印图
void LookAll(AMGraph *g){
    printf("遍历图中\n");
    for(int v=0;v<g->vexnum;v++){
        for(int w=0;w<g->vexnum;w++){
            if(g->arcs[v][w]!=MAXINT){
                printf("<%d,%d>=%d\n",v,w,g->arcs[v][w]);
            }
        }
    }
    for(int i=0;i<g->vexnum;i++){
        ElemType e = g->vexs[i];
        printf("uid=%d,name=%s,price=%f\n",e.uid,e.name,e.price);
    }
}

int main(int argc, char const *argv[])
{
    //准备图书数据
    Book book[7];
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
    AMGraph *g = initG(6);

    //检验增加功能
    for(int i=0;i<6;i++){
        addArc(g,edge[i],false);
        addVex(g,book[i],i);
    }
    LookAll(g);
    printf("***************************\n");

    //检验DFS
    printf("检验DFS\n");
    initVisited(6);
    AM_DFS(g,1);
    printf("***************************\n");

    //检验BFS
    printf("检验BFS\n");
    initVisited(6);
    AM_BFS(g,1);
    printf("***************************\n");

    //为Dijkstra算法和Floyd算法准备新的数据
    book[6].uid = 48;
    book[6].price = 16;
    strcpy(book[6].name,"图书7");

    Edge newEdge[12];
    newEdge[0].v1 = 0;
    newEdge[1].v1 = 0;
    newEdge[2].v1 = 2;
    newEdge[3].v1 = 3;
    newEdge[4].v1 = 2;
    newEdge[5].v1 = 3;
    newEdge[6].v1 = 1;
    newEdge[7].v1 = 3;
    newEdge[8].v1 = 6;
    newEdge[9].v1 = 1;
    newEdge[10].v1 = 4;
    newEdge[11].v1 = 3;

    newEdge[0].v2 = 1;
    newEdge[1].v2 = 3;
    newEdge[2].v2 = 0;
    newEdge[3].v2 = 2;
    newEdge[4].v2 = 5;
    newEdge[5].v2 = 5;
    newEdge[6].v2 = 3;
    newEdge[7].v2 = 6;
    newEdge[8].v2 = 5;
    newEdge[9].v2 = 4;
    newEdge[10].v2 = 6;
    newEdge[11].v2 = 4;
    newEdge[0].weight = 2;
    newEdge[1].weight = 1;
    newEdge[2].weight = 4;
    newEdge[3].weight = 2;
    newEdge[4].weight = 5;
    newEdge[5].weight = 8;
    newEdge[6].weight = 3;
    newEdge[7].weight = 4;
    newEdge[8].weight = 1;
    newEdge[9].weight = 10;
    newEdge[10].weight = 6;
    newEdge[11].weight = 2;

    g = initG(7);

    for(int i=0;i<7;i++){
        addVex(g,book[i],i);
    }

    for(int i=0;i<12;i++){
        addArc(g,newEdge[i],true);
    }

    //检验查找最短路径的Dij算法
    printf("检验Dijkstra算法\n");
    ShortestPath_Dij(g,0);
    printf("***************************\n");

    //检验查找最短路径的Floyd算法
    printf("检验Floyd算法\n");
    ShortestPath_Floyd(g);

    //为Prim算法准备新数据
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

    //检验生成MST的Prim算法
    PrimMST(g);
    printf("***************************\n");
    


    return 0;
}
