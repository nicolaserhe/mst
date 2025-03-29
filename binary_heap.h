#ifndef __BINARY_HEAP_H__
#define __BINARY_HEAP_H__

#include "graph.h"

struct vertex       //Prim算法需要用到的顶点的额外信息
{
    int id;         //顶点的编号
    int key;        //key保存的是当前顶点与树最短那条桥边最的权重
    int parent;     //parent保存的是当前节点在树中的父节点
};

struct heap             //二叉堆
{
    struct vertex* array;
    int* indexs;    //存放顶点对应的下标的数组，如果下标值为-1，证明该节点不在堆中
    int size;
};

struct heap* creat_heap(struct graph* graph);      //用图初始化二叉堆
//计算临近节点的下标
int parent(int index);
int left(int index);
int right(int index);
void min_heapify(struct heap* _heap, int index);        //维护堆的性质
void exchange(struct heap* _heap, int i, int j);        //交换下标i和j中的元素
void build_min_heap(struct heap* _heap);        //构建最小堆
struct vertex extract_min(struct heap* _heap);        //返回堆中最小的元素
void decrease_key(struct heap* _heap, struct vertex* x, int k); //将x的关键字减小为k
int is_empty(struct heap* _heap);      //判断堆是否为空
int exist(struct heap* _heap, struct vertex* v);   //判断v顶点是否在Q中存在，如果存在返回1，不存在返回0
struct vertex* get_node(struct heap* _heap, int v);      //得Q中存储顶点v的节点
#endif