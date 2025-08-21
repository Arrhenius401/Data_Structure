#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>


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
    /* data */
    ElemType *elem;
    //struct LinkNode 是一个已经明确声明了的存在
    //它告诉编译器：“我们正在谈论一个名为 LinkNode 的结构体类型，而这个类型稍后会在这个作用域内被完整定义。”
    struct LinkNode *next;   
}LinkNode;

typedef LinkNode *LinkList;



//1初始化
bool InitList(LinkList L){
    //构建一个空的单列表
    //生成新结点作为头结点，用头指针L指向头结点,同时头结点的数据域是空的
    L = (LinkList)malloc(sizeof(LinkNode));
    
    //内存分配失败
    if(L == NULL){
        printf("InitList函数存在问题！\n");
        return false;
    }

    //头结点的指针置空
    L->next = NULL;

    return true;
}

//2.1,按序号查找
LinkNode *FindKth(int k,LinkList L){
    LinkNode *p=L->next;
    int i = 1;
    while(p != NULL&&i<k){
        p = p->next;
        i++;
    }
    if(i==k){
        return p;
    }else{
        return NULL;
    }
}

//2.2,按值查找
LinkNode *Find(LinkList L,int uid){
    //在带头结点的单链表L中查找值为b的元素
    LinkNode *p=L->next;
    while (p!=NULL)
    {
        if(uid==p->elem->uid){
            return p;
        }
        p = p->next;
    }
    return NULL;
}


//3,插入
bool ListInsert(LinkList L,int i,Book b){
    //在带头结点的单链表L中的i个位置插入值为b的新结点
    //浙大版(改版),参数i始终是1时即是前插法
    LinkNode *p,*s;
    if(i==1){
        s = (LinkNode*)malloc(sizeof(LinkNode));
        //给结点的数据域赋值
        //b是一个生命周期受限的局部变量,直接赋值将导致结点指向不再有效的地址,最终导致未定义行为
        s->elem = (Book*)malloc(sizeof(Book));   
        *s->elem = b;
        s->next = L->next;
        L->next = s;
        return true;
    }
    p = FindKth(i-1,L);
    if(p==NULL){
        return false;
    }else{
        s = (LinkNode*)malloc(sizeof(LinkNode));
        s->elem = (Book*)malloc(sizeof(Book));
        *s->elem = b;
        s->next = p->next;
        p->next = s;
        return true;
    } 
}

//4,删除
bool Delete(LinkList L,int i){
    LinkNode *p,*s;
    p = L->next;
    if(i == 1){
        if(p!=NULL){
            s = p->next;
            L->next = s->next;
            free(s);
        }
        return true;
    }
    p = FindKth(i-1,L);
    if(p ==NULL){
        return false;
    }else if(p->next == NULL){
        return false;
    }else{
        s=p->next;
        p->next=s->next;
        free(s);
        return true;
    } 
}

//5,修改
bool modify(LinkList L,int i,ElemType b){
    LinkNode *p,*s;
    if(i==1){
        s = L->next;
        if(s==NULL){
            return false;
        }
        *s->elem = b;
        return true;
    }
    p = FindKth(i-1,L);
    if(p==NULL){
        return false;
    }else if(p->next==NULL){
        return false;
    }else{
        s = p->next;
        *s->elem = b;
        return true;
    }
}

//平衡树:左右子树高度差不大于1

//打印链表
void LookAll(LinkList L){
    LinkNode *p = L->next;
    Book *newB;
    printf("遍历链表中\n");
    while (p!=NULL)
    {
        newB=p->elem;
        printf("uid=%d,name=%s,price=%f\n",newB->uid,newB->name,newB->price);
        p = p->next;
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
    LinkList L,backup;
    if(InitList(L)){
        printf("初始化功能正常\n");
    }else{
        printf("初始化功能异常\n");
    }
    backup=L;
    printf("***************************\n");

    //检验插入(序号1)
    for(int i=0;i<3;i++){
        ListInsert(L,1,book[i]);
    }
    LookAll(L);
    printf("***************************\n");
    
    //检验插入(其他序号)
    ListInsert(L,2,book[3]);
    LookAll(L);
    printf("***************************\n");

    //检验序号查找
    LinkNode *pointL = FindKth(2,L);
    if(pointL!=NULL){
        Book *newB=pointL->elem;
        printf("uid=%d,name=%s,price=%f\n",newB->uid,newB->name,newB->price);
    }else{
        printf("序号序号功能异常\n");
    }
    printf("***************************\n");
    
    //检验按值查找
    pointL = NULL;
    pointL = Find(L,1);
    if(pointL!=NULL){
         Book *newB=pointL->elem;
        printf("uid=%d,name=%s,price=%f\n",newB->uid,newB->name,newB->price);
    }else{
        printf("按值序号功能异常\n");
    }
    printf("***************************\n");

    //检验删除
    printf("删除前\n");
    LookAll(L);
    printf("删除后\n");
    if(Delete(L,2)){
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

