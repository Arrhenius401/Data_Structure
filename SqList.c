#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>

#define MAXSIZE 1000

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
    ElemType *elem[MAXSIZE];
    int length;
}SqList;



//1,初始化
SqList *InitList(){
    SqList *L;
    L = (SqList*)malloc(sizeof(SqList));
    //顺序表里也需要动态分配内存(与链表的随用随取的动态分配不同)
    //故此处不给elem赋值
    L->length=0;
    return L;
}

//2.1,按序号查找
ElemType* FindKth(SqList *L,int i){
    if(i<1||i>L->length){
        return NULL;
    }
    Book *e = L->elem[i-1];
    return e;
}

//2.2,按值查找
int Find(SqList *L,int uid){
    for(int i=0;i<MAXSIZE;i++){
        if(uid==L->elem[i]->uid){
            return i+1;
        }
    }
    return -1;
}

//3,插入
bool ListInsert(SqList *L,int i,ElemType e){
    //在顺序表L中第i个位置插入新的元素e,i值的合法范围是1<=i<=length+1
    //检查i是否在合法范围内
    if(i<1||i>L->length+1){
        return false;
    }
    //检查链表是否满载
    if(L->length==MAXSIZE){
        return false;
    }

    for(int j=L->length-1;j>=i-1;j--){
        L->elem[j+1]=L->elem[j];
    }
    L->elem[i-1]=(Book*)malloc(sizeof(Book));
    *L->elem[i-1]=e;
    L->length++;
    return true;
}

//4,删除
bool Delete(SqList *L,int i){
    //检查i是否在合法范围内
    if(i<1||i>L->length+1){
        return false;
    }
    for(int j=i;j<L->length-1;j++){
        L->elem[j-1]=L->elem[j];
    }
    L->elem[L->length-1]=NULL;
    L->length--;
    return true;
}

//5,修改
bool modify(SqList *L,int i,ElemType e){
    //检查i是否在合法范围内
    if(i<1||i>L->length+1){
        return false;
    }
    *L->elem[i-1] = e;
    return true;
}

//打印链表
void LookAll(SqList *L){
    ElemType *e;
    for(int i=0;i<L->length;i++){
        e=L->elem[i];
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

    book[3].uid = 4;
    book[3].price = 20;
    strcpy(book[3].name,"图书4");

    //检验初始化
    SqList *L = InitList();
    if(L!=NULL){
        printf("初始化功能正常\n");
    }else{
        printf("初始化功能异常\n");
    }
    printf("***************************\n");

    //检验插入
    for(int i=0;i<3;i++){
        ListInsert(L,i+1,book[i]);
    }
    LookAll(L);
    printf("***************************\n");

    //检验按序号查找
    Book *pointB = FindKth(L,2);
    if(pointB!=NULL){
        printf("uid=%d,name=%s,price=%f\n",pointB->uid,pointB->name,pointB->price);
    }else{
        printf("查找功能异常\n");
    }
    printf("***************************\n");

    //检验按值查找
    pointB = NULL;
    int pointN = Find(L,3);
    pointB = FindKth(L,pointN);
    if(pointB!=NULL){
        printf("uid=%d,name=%s,price=%f\n",pointB->uid,pointB->name,pointB->price);
    }else{
        printf("查找功能异常\n");
    }
    printf("***************************\n");

    //检验删除
    printf("删除前\n");
    ListInsert(L,4,book[3]);
    LookAll(L);
    printf("删除后\n");
    if(Delete(L,4)){
        LookAll(L);
    }else{
        printf("删除功能异常\n");
    }
    printf("***************************\n");

    //检验修改
    printf("修改前\n");
    LookAll(L);
    printf("修改后\n");
    if(modify(L,2,book[3])){
        LookAll(L);
    }else{
        printf("修改功能异常\n");
    }
    printf("***************************\n");

    return 0;
}
