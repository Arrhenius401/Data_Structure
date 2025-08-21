#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>

#define MAXSIZE 5   //不必添加封号

//建造图书元素
typedef struct
{
    /* data */
    int uid;
    char name[20];
    float price;
}Book;

typedef Book ElemType;

typedef struct{
        ElemType *elem;
        struct LinkNode *next;
}LinkNode;

typedef struct{
    LinkNode *rear;
    LinkNode *front;
}LinkQueue;



//1,初始化
LinkQueue *initQueue(){
    LinkQueue *q = (LinkQueue*)malloc(sizeof(LinkQueue));
    //制作数据域为空的头指针
    LinkNode *n = (LinkNode*)malloc(sizeof(LinkNode));
    n->elem = NULL;
    n->next = NULL;
    q->front = n;
    q->rear = n;
    return q;
}

//2,入队列
void addQ(LinkQueue *q,ElemType e){
    LinkNode *n = (LinkNode*)malloc(sizeof(LinkNode));
    n->elem = (ElemType*)malloc(sizeof(ElemType));
    *n->elem = e;
    n->next = NULL;
    
    q->rear->next = n;
    q->rear = n;
}

//3,出队列
ElemType *deleteQ(LinkQueue *q){
    if(q->front==q->rear){
        return NULL;
    }
    LinkNode *n = q->front->next;
    ElemType *e = n->elem;
    q->front->next = n->next;
    //如果最后一个节点被删,队尾指针指向头结点
    if(q->rear == n){
        q->rear = q->front;
    }
    free(n);
    return e;
}

//打印队列
void LookAll(LinkQueue *q){
    while (q->front!=q->rear)
    {
        ElemType *e = deleteQ(q);
        if(e==NULL){
            printf("出栈功能异常\n");
            break;
        }
        printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);
    }
    
}



int main(int argc, char const *argv[])
{
    //准备图书数据
    Book book[6];
    book[0].uid = 1;
    book[1].uid = 2;
    book[2].uid = 3;
    book[3].uid = 4;
    book[4].uid = 5;
    book[5].uid = 6;
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

    //检验入队列
    LinkQueue *q = initQueue();
    for(int i=0;i<6;i++){
        addQ(q,book[i]);
    }
    printf("***************************\n");

    //检验出队列
    LookAll(q);


    return 0;
}
