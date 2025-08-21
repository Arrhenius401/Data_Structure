#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>


//二叉搜索树

//建造图书元素
typedef struct
{
    /* data */
    int uid;
    char name[20];
    float price;
}Book;

typedef Book ElemType;

typedef struct BiTree
{
    ElemType *elem;
    struct BiTree *lchild,*rchild;
}treeNode;

typedef treeNode *BiTree;



//1,初始化
BiTree initTree(){
    BiTree t = NULL;
    return t;
}

//2.1,采用迭代查找
treeNode *find(BiTree t,int uid){
    //查找失败
    if(t==NULL){
        return NULL;
    }
    if(uid > t->
    elem->uid){
        return find(t->rchild,uid);
    }else if(uid < t->elem->uid){
        return find(t->lchild,uid);
    }else{
        return t;
    }
}

//2.2,采用迭代查找
treeNode *IterFind(BiTree t,int uid){
    //由于非递归函数的执行效率高,可将"尾递归"函数改为迭代函数
    while(t){
        if(uid>t->elem->uid){
            t = t->rchild;
        }else if (uid < t->elem->uid)
        {
            t = t->lchild;
        }else{
            return t;
        }
    }
    return NULL;
}

//2.3,查找最大值
treeNode *findMax(BiTree t){
    //最终结果是最左叶节点
    if(!t){
        return NULL;
    }else if(!t->rchild){
        return t;
    }else{
        return findMax(t->rchild);
    }
}


//2.4,查找最小值
treeNode *findMin(BiTree t){
//最终结果是最右叶节点
    if(!t){
        return NULL;
    }else if(!t->lchild){
        return t;
    }else{
        return findMin(t->lchild);
    }
}

//3,插入
treeNode *insertT(BiTree tree,ElemType e){
    //调用该函数时,应当使用一个容器存储返回值
    //否则函数结束后,tree的值将保持不变
    if(tree==NULL){
        //若原树为空,则生成并返回一个节点的二叉搜索树
        tree = (treeNode*)malloc(sizeof(treeNode));
        tree->elem = (ElemType*)malloc(sizeof(ElemType));
        *tree->elem = e;
        tree->lchild = tree->rchild =NULL;
    }else{
        if(e.uid > tree->elem->uid){
            tree->rchild = insertT(tree->rchild,e);
        }else if(e.uid < tree->elem->uid){
            tree->lchild = insertT(tree->lchild,e);
        }
    }
    return tree;

}
//4,删除
treeNode *delete(BiTree t,int uid){
    if(t==NULL){
        return NULL;
    }else if(uid < t->elem->uid){
        //左子树递归删除
        t->lchild = delete(t->lchild,uid);
    }else if(uid > t->elem->uid){
        //右子树递归删除
        t->rchild = delete(t->rchild,uid);
    }else{
        //找到要删除的结点
        if(t->lchild!=NULL&&t->rchild!=NULL){
            //结点有左右儿子
            //在右子树中找最小元素填充删除结点
            treeNode *s = findMin(t->rchild);
            t->elem = s->elem;
            //在删除结点的右子树中删除最小元素
            t->rchild = delete(t->rchild,t->elem->uid);
        }else{
            //结点最多有一个儿子
            treeNode *s = t;
            //结点有一个儿子
            if(t->lchild != NULL){
                t = t->lchild;
            }else if(t->rchild != NULL){
                t = t->rchild;
            }else{
                //是叶节点
                t = NULL;
            }
            free(s);
        }

    }
    return t;
}

//打印树(中序遍历)
void inOrderTraverse(BiTree t){
    if(t!=NULL){
        inOrderTraverse(t->lchild);
        ElemType *e = t->elem;
        printf("uid=%d,name=%s,price=%f\n",e->uid,e->name,e->price);
        inOrderTraverse(t->rchild);
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

    //检验插入功能
    BiTree t = initTree();

    for(int i=0;i<6;i++){
        t=insertT(t,book[i]);
    }
    inOrderTraverse(t);
    printf("***************************\n");

    //检验删除功能
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
        t=insertT(t,book[i]);
    }
    printf("删除前\n");
    inOrderTraverse(t);
    printf("***************************\n");

    printf("删除图书12(叶节点)\n");
    t = delete(t,60);
    inOrderTraverse(t);
    printf("***************************\n");

    printf("删除图书2(有一个子节点)\n");
    t = delete(t,18);
    inOrderTraverse(t);
    printf("***************************\n");

    printf("删除图书4(有两个子节点)\n");
    t = delete(t,24);
    inOrderTraverse(t);
    printf("***************************\n");

    return 0;
}

