#ifndef __FIBONACCI_HEAP_H__
#define __FIBONACCI_HEAP_H__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"
#define TRUE 1
#define FALSE 0


struct vertex       //Prim算法需要用到的顶点的额外信息
{
    int id;         //顶点的编号
    int key;        //key保存的是当前顶点与树最短那条桥边最的权重
    int parent;     //parent保存的是当前节点在树中的父节点
};

struct node
{
    struct node* p;                     //节点的父节点
    struct node* child;                 //节点的孩子节点
    struct node* left;                  //节点的左兄弟
    struct node* right;                 //节点的右兄弟
    int degree;                         //节点的度
    int mark;                           //节点是被标记（如果节点失去一个孩子，那么节点被标记）
    int key;                            //节点的关键字
    int id;
    int parent;
};

struct heap
{
    struct node* min;                   //保存斐波那契堆中关键字最小的节点
    int size;                           //保存斐波那契堆中节点的数目
    int *flag;                          //标记顶点是否在斐波那契堆中
    struct node** address;              //保存每个顶点的地址
    struct node* nil;                   //根链表的空节点
    int capacity;                       //斐波那契堆的容量
};

struct heap* make_heap();           //创建一个斐波那契堆
struct heap* creat_heap(struct graph* _graph);          //用图创建斐波那契堆
struct node* alloc_node(struct heap* _heap, int vertex);        //包装节点
struct node* get_node(struct heap* _heap, int v);       //返回顶点v的包装节点
int exist(struct heap* _heap, struct node* v);    //判断节点是否存在
int is_empty(struct heap* _heap);          //判断堆是否为空
void decrease_key(struct heap* h, struct node* x, int k);  //适配heap_decrease_key函数


void insert(struct heap* h, struct node* x);    //插入一个节点
struct node* minimum(struct heap* h);       //返回斐波那契堆中具有最小关键字的元素的指针
struct heap* heap_union(struct heap* h1, struct heap* h2);  //合并两个堆，注销堆h1和h2，返回合并后的堆
struct node* extract_min(struct heap* h);       //将最小节点从斐波那契堆中删除，并返回最小节点
void consolidate(struct heap* h);           //合并斐波那契堆的根链表
int fibonacci_heap_degree(int n);           //计算斐波那契堆的最大度
void heap_link(struct heap* h, struct node* y, struct node* x);     //将y节点链接到x节点上
void heap_decrease_key(struct heap* h, struct node* x, int k);  //减小某个节点的关键字
void cut(struct heap* h, struct node* x, struct node* y);   //把x从y上剪掉
void cascading_cut(struct heap* h, struct node* y);       //级联切断
#endif
