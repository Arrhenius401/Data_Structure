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

typedef struct {
    ElemType *elem[MAXSIZE];
    int top;
}SqStack;



//1,初始化
SqStack *initStack(){
    //top参数表示栈内已堆积的元素个数
    SqStack *L = (SqStack*)malloc(sizeof(SqStack));
    L->top = 0;
    return L;
}

//2,入栈
bool Push(SqStack *L,ElemType e){
    if(L->top==(MAXSIZE)){
        return false;
    }
    L->elem[L->top] = (ElemType*)malloc(sizeof(ElemType));
    *L->elem[L->top] = e;
    L->top++;
    return true;
}

//3,出栈
ElemType* Pop(SqStack *L){
    if(L->top==0){
        return NULL;
    }
    ElemType *e=L->elem[L->top-1];
    L->top--;
    return e;
}

//打印栈
void LookAll(SqStack *L){
    int top = L->top;
    for(int i=0;i<top;i++){
        ElemType *e = Pop(L);
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

    //检验初始化
    SqStack *stack=initStack();
    if(stack!=NULL){
        printf("初始化功能正常\n");
    }else{
        printf("初始化功能异常\n");
    }
    printf("***************************\n");
    
    //检验入栈和出栈
    for(int i=0;i<3;i++){
        Push(stack,book[i]);
    }
    LookAll(stack);
    printf("***************************\n");

    return 0;
}
