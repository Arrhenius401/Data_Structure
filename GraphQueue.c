#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>
#include "GraphQueue.h"


//该文件用于图的BFS
typedef struct{
    int num;
    struct node *next;
}node;

typedef struct 
{
    node *front,*rear;
}GraphQueue;



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



/* int main(int argc, char const *argv[])
{
    printf("hello world\n");
    GraphQuene *q = initGQ();
    if(isQueneEmpty(q)){
        printf("队列为空\n");
    }

    return 0;
} */
