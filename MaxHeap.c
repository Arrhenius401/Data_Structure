#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>

#define MAXDATA 1000



//树的变种--最大堆(顺序存储)
//在堆中,第0个结点只有第1个结点作为儿子,第1个结点有第2和第3两个结点

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
    int size;       //堆的当前元素个数
    int capacity;   //堆的最大容量
}Heap;

typedef Heap *MaxHeap;



//1,初始化
MaxHeap initHeap(int MaxSize){
    //创建容量为MaxSize的空的最大堆
    MaxHeap h = (MaxHeap)malloc(sizeof(Heap));
    h->elem = (ElemType*)malloc(MaxSize*sizeof(ElemType));
    h->size = 0;
    h->capacity = MaxSize;
    //定义哨兵为大于堆中所有可能元素的值,便于以后更快操作
    h->elem[0].uid = MAXDATA;
    strcpy(h->elem->name,"哨兵");
    h->elem->price = 9999999;
    return h;
}

//2,插入
bool insertH(MaxHeap h,ElemType e){
    //将元素e插入最大堆h,其中h->elem[0]已被定义为哨兵
    int i;
    if(h->size == (h->capacity+1)){
        return false;
    }
    //i指向插入堆中的最后一个元素的后一个位置
    //通过迭代i最终指向应该插入的位置
    i = ++h->size;
    for(;(h->elem+i/2)->uid < e.uid;i=i/2){
        //若父结点小于插入的结点,则将将父结点放到它空着的子结点处
        //原父结点位置留给插入的结点
        h->elem[i] = h->elem[i/2];
    }
    h->elem[i] = e;

    return true;
}

//3,删除
ElemType *deleteMax(MaxHeap h){
    //取出根结点(最大值)元素,同时删除堆的一个节点
    if(h->size == 0){
        return NULL;
    }
    //取出根结点最大值
    ElemType *maxItem = (ElemType*)malloc(sizeof(ElemType));
    *maxItem = *(h->elem+1);


    //调整树
    int parent,child;
    //用最大堆中最后一个元素从根节点开始向上过滤下层结点
    ElemType *temp = h->elem+h->size;
    h->size--;
    for(parent = 1;parent*2 <= h->size;parent = child){
        child = parent*2;
        if((child != h->size)&&((h->elem+child)->uid < (h->elem+child+1)->uid)){
            //找到左右子结点中较大者
            child++;
        }
        if(temp->uid > (h->elem+child)->uid){
            break;
        }else{
            //将较大的子结点替换其原父节点
            //同时移动temp元素到下一层
            *(h->elem+parent) = *(h->elem+child);
        }
    }
    //由于无法确保在上面的循环中能安置好堆的最后一个元素
    //所以需要特别安排最后一个元素的归宿
    //别放break前面
    *(h->elem+parent) = *temp;
    return maxItem;
}

//打印堆
void inOrderTraverse(MaxHeap h){
    for(int i=0;i<h->size+1;i++){
        ElemType *e = (h->elem+i);
        printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);
    }    
}

int main(int argc, char const *argv[])
{
    //准备图书数据
    Book book[6];
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

    //检验初始化
    MaxHeap h = initHeap(100);

    //检验插入
    for(int i=0;i<6;i++){
        insertH(h,book[i]);
    }
    inOrderTraverse(h);
    printf("***************************\n");

    //检验删除
    //继续添加图书数据
    book[0].uid = 13;
    book[1].uid = 22;
    book[2].uid = 23;
    book[3].uid = 25;
    book[4].uid = 30;
    book[5].uid = 60;
    book[0].price = 123;
    book[1].price = 46;
    book[2].price = 77;
    book[3].price = 131;
    book[4].price = 42;
    book[5].price = 59;
    strcpy(book[0].name,"图书7");
    strcpy(book[1].name,"图书8");
    strcpy(book[2].name,"图书9");
    strcpy(book[3].name,"图书10");
    strcpy(book[4].name,"图书11");
    strcpy(book[5].name,"图书12");

    for(int i=0;i<6;i++){
        insertH(h,book[i]);
    }
    printf("删除前\n");
    inOrderTraverse(h);
    printf("***************************\n");

    printf("第一次删除后\n");
    ElemType *e = deleteMax(h);
    printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);

    printf("总览\n");
    inOrderTraverse(h);
    printf("***************************\n");

    printf("第二次删除后\n");
    e = deleteMax(h);
    printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);

    printf("总览\n");
    inOrderTraverse(h);
    printf("***************************\n");

    return 0;
}
