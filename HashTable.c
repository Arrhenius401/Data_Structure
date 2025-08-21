#include <stdio.h>  
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h>



//通用设计
#define TABLE_SIZE 20   //散列表大小
typedef int KeyType;    //关键字类型
typedef int ValueType;  //数据类型
//散列单元状态类型,共有合法、空闲、删除三种类型
typedef enum{Legal,Empty,Deleted} Entrytype;



//存放散列单元数据的单独结点
typedef struct{
    Entrytype state;
    KeyType key;
    ValueType value;
}HashCell;



//开放地址法
//单独结点构成的散列表实体
typedef struct{
    int TableSize;  //散列表的最大长度
    HashCell *cell; //存放散列单元数据的数组
}HashEntity_OPEN;

//包装成指针类型
typedef HashEntity_OPEN *HashTable_OPEN;



//0,自定义的哈希函数h(k)=key%13
int HashFunction(KeyType key){
    int position = key%13;
    return position;
}

//1,创建和初始化散列表
HashTable_OPEN initHashTable_OPEN(int TableSize){
    //此处TableSize从1开始
    HashTable_OPEN ht = (HashTable_OPEN)malloc(sizeof(HashEntity_OPEN));

    //为ht里内容物赋值
    ht->TableSize = TableSize;
    ht->cell = (HashCell*)malloc(sizeof(HashCell)*TableSize);
    for(int i=0;i<TableSize;i++){
        ht->cell[i].state = Empty;
    }

    return ht;
}

//2,开放地址法处理冲突
//此函数用于找到key的位置,或者返回一个空位置
int Find_OPEN(HashTable_OPEN ht,KeyType key){
    int NewPos,CurrentPos;              //当前位置和索引位置
    NewPos = CurrentPos = HashFunction(key);
    int count=1;    //矛盾次数
    while(ht->cell[NewPos].key != key&&ht->cell[NewPos].state != Empty){
        //矛盾次数为奇数时,索引位置加(count+1)/2的平方
        //矛盾次数为偶数时,索引位置减(count/2)的平方
        //当得到的NewPos超出合法范围,则调整NewPos
        if(count%2){
            NewPos = CurrentPos + (count+1)*(count+1)/4;
            if(NewPos >= ht->TableSize){
                NewPos = NewPos % ht->TableSize;
            }
        }else{
            NewPos = CurrentPos - count*count/4;
            while(NewPos < 0){
                NewPos += ht->TableSize;
            }
        }
        count++;
    }

    //循环结束,意味着找到key的位置,或者空位置
    return NewPos;
}

//3,插入函数'
bool Insert_OPEN(HashTable_OPEN ht,KeyType key,ValueType value){
    int pos = Find_OPEN(ht,key);

    if(ht->cell[pos].state != Legal){
        //删除和空闲状态的单元可以被使用
        ht->cell[pos].state = Legal;
        ht->cell[pos].value = value;
        ht->cell[pos].key = key;
        return true;
    }else{
        return false;
    }
}

//4,删除函数
//本质是确保散列表内没有这个key,因此合法单元被删除、空闲、删除三种状态都可以,故采用
void Delete_OPEN(HashTable_OPEN ht,KeyType key){
    int pos = Find_OPEN(ht,key);

    if(ht->cell[pos].state == Legal){
        ht->cell[pos].state = Deleted;
    }
}

//5,摧毁哈希表,释放空间
void Destroy_OPEN(HashTable_OPEN ht){
    for(int i=0;i<ht->TableSize;i++){

    }
    free(ht);
}

//遍历散列表
void LookAll_OPEN(HashTable_OPEN ht){
    for(int i=0;i < ht->TableSize;i++){
        if(ht->cell[i].state == Legal){
            printf("第%d位单元存放:key=%d value=%d\n",i,ht->cell[i].key,ht->cell[i].value);
        }
    }
}



//链地址法
//和链表一样,随用随取,没有上限
//引入辅助数据结构——链表
typedef struct{
    KeyType key;
    ValueType value;
    struct LinkNode *next;
}LinkNode;

//包装成指针类型
typedef LinkNode *LinkList;
//指定返回的地址是结点指针
typedef LinkNode *Position;

//散列表实体
typedef struct{
    int TableSize;      //散列表的最大长度(其实是索引值的数量？)
    LinkList heads;     //指向链表头结点的数组
}HashEntity_Link;

//包装成指针类型
typedef HashEntity_Link *HashTable_Link;

//1,创建和初始化散列表
HashTable_Link initHashTable_Link(int TableSize){
    HashTable_Link ht = (HashTable_Link)malloc(sizeof(HashEntity_Link));
    
    //初始化表头结点
    //头结点仍设置为数据域为空的结点
    ht->TableSize = TableSize;
    ht->heads = (LinkList)malloc(sizeof(LinkNode)*TableSize);
    for(int i=0;i<20;i++){

        ht->heads[i].key = ht->heads[i].value = -1;
        ht->heads[i].next = NULL;

    }

    return ht;
}

//2,查找函数
Position Find_Link(HashTable_Link ht,KeyType key){
    //通过哈希函数得到索引值
    int pos = HashFunction(key);

    Position p = ht->heads[pos].next;
    while (p!=NULL)
    {
        if(p->key == key){
            break;
        }
        p = p->next;
    }

    //要么返回地址,表示key找到;要么返回NULL,表示key无法知道
    return p;
}

//3,插入函数
bool Insert_Link(HashTable_Link ht,KeyType key,ValueType value){
    Position p = Find_Link(ht,key);

    if(!p){
        //键值不存在,可以插入
        //哈希函数找到索引值
        int pos = HashFunction(key);
        //准备新结点的数据
        p = (Position)malloc(sizeof(LinkNode));
        p->key = key;
        p->value = value;
        //采用头插入
        p->next = ht->heads[pos].next;
        ht->heads[pos].next = p;

        return true;
    }else{
        //键值存在,不可插入
        return false;
    }
}

//遍历散列表
void LookAll_Link(HashTable_Link ht){
    for(int i=0;i < ht->TableSize;i++){
        Position p = ht->heads[i].next;
        while (p)
        {
            printf("第%d位单元存放:key=%d value=%d\n",i,p->key,p->value);
            p = p->next;
        }
    }

}

int main(int argc, char const *argv[])
{
    //准备基础数据
    HashCell cells[13];
    cells[0].key = cells[0].value = 19;
    cells[1].key = cells[1].value = 14;
    cells[2].key = cells[2].value = 23;
    cells[3].key = cells[3].value = 1;
    cells[4].key = cells[4].value = 68;
    cells[5].key = cells[5].value = 20;
    cells[6].key = cells[6].value = 84;
    cells[7].key = cells[7].value = 27;
    cells[8].key = cells[8].value = 55;
    cells[9].key = cells[9].value = 11;
    cells[10].key = cells[10].value = 10;
    cells[11].key = cells[11].value = 79;
    cells[12].key = cells[12].value = 99;


    //开放地址法
    //检验初始化函数
    printf("检验开放地址法\n");
    HashTable_OPEN ho = initHashTable_OPEN(TABLE_SIZE);

    //检验插入函数和查找函数
    for(int i=0;i<13;i++){
        Insert_OPEN(ho,cells[i].key,cells[i].value);
    }
    LookAll_OPEN(ho);
    printf("***************************\n");    

    Destroy_OPEN(ho);

    //链地址法
    //检验初始函数
    printf("检验链地址法\n");
    HashTable_Link hl = initHashTable_Link(TABLE_SIZE);


    //检验插入和查找函数
    for(int i=0;i<13;i++){
        Insert_Link(hl,cells[i].key,cells[i].value);
    }
    LookAll_Link(hl);
    printf("***************************\n");  



    return 0;
}
