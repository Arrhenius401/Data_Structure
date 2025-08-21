#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>



typedef int ElementType;


//0.1,交换两元素位置
void Swap(ElementType *x,ElementType *y){
    ElementType temp;

    temp = *y;
    *y = *x;
    *x = temp;
}

//0.2,打印序列
void PrintArray(ElementType elem[],int N){
    printf("数列为:");
    for(int i=0;i<N;i++){
        printf("%d ",elem[i]);
    }
    printf("\n");
}



//1,冒泡排序
void Bubble_Sort(ElementType elem[],int N){
    for(int p=N-1;p>=0;p--){
        int flag = 0;       //用于确定一趟排序后,是否仍需交换
        for(int i=0;i<p;i++){   //一趟冒泡
            if(elem[i]>elem[i+1]){
                Swap(&elem[i],&elem[i+1]);
                flag = 1;   //标识发生了交换
            }
        }
        if(flag == 0){
            break;
        }
    }
}



//2,插入排序
void Insertion_Sort(ElementType elem[],int N){
    for(int p=1;p<N;p++){
        //视旧有牌为elem[0]~elem[p-1],新牌为elem[p]
        ElementType temp = elem[p];     //摸下一张牌
        int i = 0;
        for(i=p;i>0&&elem[i-1]>temp;i--){
            elem[i] = elem[i-1];        //大牌后移
        }
        elem[i] = temp;     //新牌就位
    }
}



//3,希尔排序(采用Sedgewick增量序列)
void Shell_Sort(ElementType elem[],int N){
    //只使用一小部分增量
    int Sedgewick[] = {929,505,209,109,41,19,5,1,0};

    //初始的增量Sedgewick[si]不能超过待排序列长度
    int si;
    for(si=0;Sedgewick[si]>=N;si++);

    for(int d=Sedgewick[si];d>0;d=Sedgewick[++si]){
        for(int p=d;p<N;p++){               //插入排序,操作对象是elem[d]~elem[N-1]
            ElementType temp = elem[p];
            int i;
            for(i=p;i>=d&&elem[i-d]>temp;i-=d){     //"i>=d"不会溢出
                elem[i] = elem[i-d];
            }
            elem[i] = temp;
        }
    }  
}



//4,堆排序
//4.1,下滤操作(percolate down)
void Percdown(ElementType elem[],int p,int N){
    //将N个元素的数组中,以elem[p]为根的子堆调整为最大堆
    int parent,child;
    ElementType temp;

    temp = elem[p];
    for(parent=p;(parent*2+1)<N;parent=child){
        child = parent*2+1;
        if(child!=(N-1)&&elem[child]<elem[child+1]){
            //elem[N-1]是堆的最后一个元素
            child++;
        }
        if(temp>=elem[child]){
            break;
        }else{
            elem[parent] = elem[child];
        }
    }
    //这步不能放break前,否则将丢失e值
    elem[parent] = temp;
}

//4.2,堆排序主代码
void HeapSort(ElementType elem[],int N){
    int p;

    for(p=N/2-1;p>=0;p--){  //建立最大堆
        Percdown(elem,p,N);
    }

    for(p=N-1;p>0;p--){     //交换堆首和堆尾(p)的元素,并调整最大堆
        Swap(&elem[0],&elem[p]);
        Percdown(elem,0,p);
    }
}




//5,归并排序
//5.1,相邻两个有序子序列的归并(递归和循环的通用代码)
void Merge(ElementType elem[],ElementType tempE[],int l,int r,int RightEnd){
    //l表示左子序列的起始位置,r表示右子序列的起始位置,end表示右子序列结束位置
    int LeftEnd = r-1;          //左子序列结束位置
    int num = RightEnd-l+1;     //左右子序列元素总数量
    int temp = l;               //

    while (l<=LeftEnd&&r<=RightEnd)
    {
        if(elem[l]<=elem[r]){
            tempE[temp++] = elem[l++];
        }else{
            tempE[temp++] = elem[r++];
        }
    }
    //循环退出,有一个子序列结束
    while (l<=LeftEnd)
    {
        tempE[temp++] = elem[l++];
    }
    while (r<=RightEnd)
    {
        tempE[temp++] = elem[r++];
    }

    //将有序的tempE[]复制回elem[]
    for(int i=0;i<num;i++,RightEnd--){
        elem[RightEnd] = tempE[RightEnd];
    }  
}

//递归实现
//5.2.1,核心递归排序函数
void Msort_Recursion(ElementType elem[],ElementType tempE[],int l,int RightEnd){
    int center;
   
    //当l==RightEnd时,Msort函数不进行操作
    //它将返回上一层Msort函数,执行Merge函数
    if(l<RightEnd){
        center = (l+RightEnd)/2;
        Msort_Recursion(elem,tempE,l,center);             //递归解决左子序列
        Msort_Recursion(elem,tempE,center+1,RightEnd);    //递归解决右子序列
        Merge(elem,tempE,l,center+1,RightEnd);  //合并两个子序列
    }
}

//5.2.2,统一接口
void Merge_Recursion_Sort(ElementType elem[],int N){
    ElementType *tempE = (ElementType*)malloc(sizeof(ElementType)*N);
    
    if(tempE!=NULL){
        Msort_Recursion(elem,tempE,0,N-1);
        free(tempE);
    } else{
        printf("空间不足\n");
    }
}


//循环实现
//5.3.1,核心循环排序函数
void Msort_Pass(ElementType elem[],ElementType tempE[],int N,int length){
    //length是当前子序列的长度

    //处理除尾巴外能成对的子序列
    int l=0;    //左子序列起始位置
    for(;l<=N-2*length;l+=2*length){
        Merge(elem,tempE,l,l+length,l+2*length-1);
    }

    //收尾处理
    if(l+length < N){
        //尾端的序列还能分为两个子序列
        Merge(elem,tempE,l,l+length,N-1);
    }else{
        //尾端的序列不可分
        for(int i=l;i<N;i++){
            tempE[i] = elem[i];
        }
    }
}

//5.3.2,统一接口
void Merge_Pass_Sort(ElementType elem[],int N){
    ElementType *tempE = (ElementType)malloc(sizeof(ElementType)*N);
    int length=1;   //初始化子序列长度

    if(tempE!=NULL){
        while (length<N)
        {
            Msort_Pass(elem,tempE,N,length);
            length *= 2;
            //将在上一个Msort_pass函数里存入tempE[]的数据再操作后,重洗导回elem[]
            Msort_Pass(tempE,elem,N,length);
            length *= 2;
        }
        free(tempE);
    }else{
        printf("空间不足\n");
    }
}



//6,快速排序
//6.1,在首中尾三个数字中寻找中位数
ElementType FindMid(ElementType elem[],int left,int right){
    int center = (left+right)/2;
    if(elem[left] > elem[center]){
        Swap(&elem[left],&elem[center]);
    }
    if(elem[left] > elem[right]){
        Swap(&elem[right],&elem[left]);
    }
    if(elem[center] > elem[right]){
        Swap(&elem[center],&elem[right]);
    }
    //此时elem[left]<=elem[center]<=elem[right]
    //将基准Pivot的elem[center]藏到右边
    Swap(&elem[center],&elem[right-1]);     //elem[right]必比elem[center]大
    return elem[right-1];
}

#define CUTOFF 2    //采用简单排序时的元素数量
//6.2,核心递归排序函数
void Qsort(ElementType elem[],int left,int right){
    if((right-left) > CUTOFF){       //递归函数的嵌套必须放在一个判断条件中,否则会陷入死循环
        int pivot = FindMid(elem,left,right);
        int low = left;
        int high = right-1;
        while (1){
            while(elem[++low] < pivot);     //找到左子堆里比基准大的数
            while(elem[--high] > pivot);    //找到右子堆里比基准小的数
            if(low<high){
                Swap(&elem[low],&elem[high]);
            }else{
                break;
            } 
        }

        Swap(&elem[low],&elem[right-1]);    //将基准换到正确位置

        printf("pivot=%d",pivot);
        PrintArray(elem,10);

        Qsort(elem,left,low-1);             //递归解决左边
        Qsort(elem,low+1,right);            //递归解决右边
    }else{
        //当数据元素少时,采用冒泡排序
        //同时解决上方left,right相差极小时,数据错位问题
        Bubble_Sort(elem+left,right-left+1);
    }
}

//6.3,统一接口
void Quick_Sort(ElementType elem[],int N){
    Qsort(elem,0,N-1);
}



//7,基数排序
#define MAXDIGIT 2  //关键字最大位,此处我们选取的是两位数序列
#define RADIX 10    //关键字基数,此时基数是十进制整数

//7.0.1,辅助数据结构——桶元素节点
typedef struct{
    int key;
    struct Node *next;
}Node;

//7.0.2,辅助数据结构——桶头结点
typedef struct{
    Node *head,*tail;
}HeadNode;

typedef HeadNode Bucket[RADIX];

//7.1,获得元素关键字(例:数字在指定位置(从右往左数第d位)上的数字)
int GetDigit(int X,int D){
    //默认次位D=1
    int d;

    for(int i=1;i<=D;i++){
        d = X%RADIX;
        X /= RADIX; 
    }
    return d;
}

//7.2,主函数
void Radix_Sort(ElementType elem[],int N){
    //初始化每个桶
    Bucket B;
    for(int i=0;i<RADIX;i++){
        B[i].head = B[i].tail = NULL;
    }

    //初始化链表list
    Node *list,*temp;
    list = NULL;
    for(int i=0;i<N;i++){
        temp = (Node*)malloc(sizeof(Node));
        temp->key = elem[i];
        temp->next = list;
        list = temp;
    }

    //开始排序
    int Di;
    for(int D=1;D<=MAXDIGIT;D++){
        Node *p = list;
        while (p!=NULL)
        {
            //获得桶序号Di和存有Elemtype数据的结点temp
            Di = GetDigit(p->key,D);
            temp = p;
            p = p->next;
            //将temp作为新节点插入B[Di]桶尾
            //
            temp->next = NULL;
            if(B[Di].head==NULL){
                B[Di].head = B[Di].tail = temp;
            }else{
                B[Di].tail->next = temp;
                B[Di].tail = temp;
            }
        }

        //将每个桶的元素顺序收集,用以重新构造list
        list = NULL;
        for(Di = RADIX-1;Di>=0;Di--){
            if(B[Di].head){
                //如果桶不为空,则整桶插入list表头
                B[Di].tail->next = list;
                list = B[Di].head;
                B[Di].head = B[Di].tail = NULL;
            }
        }
    }//结束排序

    //将list倒入elem[]并释放空间
        for (int i=0;i<N;i++)
        {
            temp = list;
            list = list->next;
            elem[i] = temp->key;
            free(temp);
        }
    
}


int main(int argc, char const *argv[])
{
    //初始化数据
    ElementType elem[10] = {99,3,56,23,44,8,75,12,5,84};
    int N = 10;
    PrintArray(elem,N);
    printf("***************************\n");

    //检验冒泡排序
/*     printf("检验冒泡排序\n");
    Bubble_Sort(elem,N);
    PrintArray(elem,N);
    printf("***************************\n"); */

    //检验插入排序
/*     printf("检验插入排序\n");
    Insertion_Sort(elem,N);
    PrintArray(elem,N);
    printf("***************************\n"); */

    //检验希尔排序
/*     printf("检验希尔排序\n");
    Shell_Sort(elem,N);
    PrintArray(elem,N);
    printf("***************************\n"); */

    //检验堆排序
/*     printf("检验堆排序\n");
    HeapSort(elem,N);
    PrintArray(elem,N);
    printf("***************************\n"); */

    //检验归并排序
/*     printf("检验递归实现的归并排序\n");
    Merge_Recursion_Sort(elem,N);
    PrintArray(elem,N);
    printf("***************************\n"); */

/*     printf("检验递归实现的循环排序\n");
    Merge_Pass_Sort(elem,N);
    PrintArray(elem,N);
    printf("***************************\n"); */

    //检验快速排序
    printf("检验快速排序\n");
    Quick_Sort(elem,N);
    PrintArray(elem,N);
    printf("***************************\n");

    //检验基数排序
/*     printf("检验基数排序\n");
    Radix_Sort(elem,N);
    PrintArray(elem,N);
    printf("***************************\n"); */

    return 0;
}
