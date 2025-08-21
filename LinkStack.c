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
    ElemType *elem;
    struct LinkNode *next;
}LinkNode;

typedef LinkNode *LinkStack;



//1,初始化
LinkStack initStack(){
    //制作数据域为空的头结点
    LinkStack L = (LinkNode*)malloc(sizeof(LinkNode));
    L->elem = NULL;
    L->next = NULL;
    return L;
}

//2,入栈
void Push(LinkStack L,ElemType e){
    LinkNode *p = (LinkNode*)malloc(sizeof(LinkNode));
    p->elem = (ElemType*)malloc(sizeof(Book));
    *p->elem = e;
    p->next = L->next;
    L->next = p;
}

//3,出栈
ElemType *Pop(LinkStack L){
    LinkNode *p = L->next;
    if(p == NULL){
        return NULL;
    }
    ElemType *e = p->elem;
    L->next = p->next;
    free(p);
    return e;
}

//打印栈
void LookAll(LinkStack L){
    while (L->next)
    {
        ElemType *e = Pop(L);
        printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);
    }
    
}


int main(int argc, char const *argv[])
{
    //准备图书数据
    Book book[4];
    book[0].uid = 1;
    book[1].uid = 2;
    book[2].uid = 3;
    book[0].price = 12;
    book[1].price = 16;
    book[2].price = 17;
    strcpy(book[0].name,"图书1");
    strcpy(book[1].name,"图书2");
    strcpy(book[2].name,"图书3");

    //检验入栈和出栈
    LinkStack stack = initStack();
    for(int i=0;i<3;i++){
        Push(stack,book[i]);
    }
    LookAll(stack);
    printf("***************************\n");


    return 0;
}
