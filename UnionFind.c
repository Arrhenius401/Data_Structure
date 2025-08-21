#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>

#define MAXSIZE 1000


//树的变种--并查集

typedef int Elemtype;

typedef struct{
    Elemtype *elem;     //存储的数据类型
    int *parent;        //存储每个节点的父节点
    int *rank;          //存储每棵树(或分支)的秩(高度),用于优化合并
    int count;          //连通分量的数量,即整个并查集里独立的树的数量
}UnionFind;

//1,初始化
UnionFind *initUF(int n){
    UnionFind *uf = (UnionFind*)malloc(sizeof(UnionFind));
    uf->parent = (int*)malloc(sizeof(int)*n);
    uf->rank = (int*)malloc(sizeof(int)*n);
    uf->count = n;

    //初始化并查集里单独的树的根节点和秩
    for(int i=0;i<n;i++){
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
    return uf;
}

//2,查找根结点操作,并进行路径压缩
int findSet(UnionFind *uf,int num){
    if(uf->parent[num] != num){
        uf->parent[num] = find(uf,uf->parent[num]); //路径压缩
    }
    return uf->parent[num];
}

//3,合并集合操作
void unionSet(UnionFind *uf,int x,int y){
    int rootX = find(uf,x);
    int rootY = find(uf,y);

    //将秩较大的树的根结点序号赋值给秩较小的树的根结点序号
    if(rootX != rootY){
        if(uf->rank[rootX] > uf->rank[rootY]){
            uf->parent[rootY] = rootX;
        }else if(uf->rank[rootX] < uf->rank[rootY]){
            uf->parent[rootX] = rootY;
        }else{
            uf->parent[rootY] = rootX;
            uf->rank[rootX]++;
        }
    }
    uf->count--;
}



