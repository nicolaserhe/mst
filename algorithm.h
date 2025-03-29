#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__
#include "disjoint_set.h"

#ifdef FIB
#include "fibonacci_heap.h"
#else
#include "binary_heap.h"
#endif

#include "graph.h"

struct mst          //根据两种不同的算法返回不同的生成树结构
{
    void* data;
    int size;
    int flag;
};

struct mst* mst_Kruskal(struct graph* _graph);     //Kruskal算法
struct mst* mst_Prim(struct graph* _graph);         //Prim算法
void quick_sort(struct e_node* _edge, int p, int r);    //快速排序
int partition(struct e_node* _edge, int p, int r);     //对子数组原址重排
void save_mst(struct mst* _mst, char* filename);    //将最小生成树保存到文件中
#endif