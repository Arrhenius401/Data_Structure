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

typedef struct 
{
    //循环列表解决假溢出的两种方法
    //1,使用额外标记:size或者tag域
    //2,仅使用n-1个数组空间
    //此处采用第二种方法
    ElemType *elem[MAXSIZE+1];
    int rear;
    int front;
}SqQueue;

//1,初始化
SqQueue *initQueue(){
    SqQueue *q = (SqQueue*)malloc(sizeof(SqQueue));
    q->front = 0;
    q->rear = 0;
    return q;
}

//2,入队列
bool addQ(SqQueue *q,ElemType e){
    if((q->rear+1)%(MAXSIZE+1)==q->front){
        return false;
    }

    q->elem[q->rear] = (ElemType*)malloc(sizeof(ElemType));
    q->rear = (q->rear+1)%(MAXSIZE+1);
    *q->elem[q->rear] = e;
    return true;
}

//3,出队列
ElemType *deleteQ(SqQueue *q){
    if(q->rear==q->front){
        return NULL;
    }
    ElemType *e = q->elem[q->front+1];
    q->front = (q->front+1)%(MAXSIZE+1);
    return e;
}

//打印队列
void LookAll(SqQueue *q){
    while (q->rear!=q->front)
    {
        ElemType *e = deleteQ(q);
        if(e == NULL){
            printf("数据错误\n");
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
    SqQueue *q = initQueue();
    for(int i=0;i<6;i++){
        if(addQ(q,book[i])){
            printf("第%d本图书添加成功\n",i+1);
        }else{
            printf("第%d本图书添加失败\n",i+1);
        }
    }
    printf("***************************\n");

    //检验出队列
    LookAll(q);

    return 0;
}
