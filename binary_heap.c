#include <stdio.h>
#include <stdlib.h>
#include "binary_heap.h"

struct heap* creat_heap(struct graph* _graph)
{
    struct heap* tmp = (struct heap*)malloc(sizeof(struct heap));
    tmp->array = (struct vertex*)malloc(sizeof(struct vertex) * _graph->vertex_size);
    tmp->indexs = (int*)malloc(sizeof(int) * _graph->vertex_size);
    for(int i = 0; i < _graph->vertex_size; i++)        //初始化堆中节点的数据
    {
        tmp->array[i].key = INT_MAX;
        tmp->array[i].parent = -1;
        tmp->array[i].id = i;
        tmp->indexs[i] = i;
    }
    tmp->array[0].key = 0;
    tmp->size = _graph->vertex_size;
    //堆中只有第一个顶点的关键字是0，其余都是INT_MAX，符合最小堆的定义
    return tmp;
}


//计算临近节点的下标
int parent(int index)
{
    return (index - 1) / 2;
}

int left(int index)
{
    return index * 2 + 1;
}

int right(int index)
{
    return (index + 1) * 2;
}

void min_heapify(struct heap* _heap, int index)
{
    int l, r, least;
    while(1)
    {
        l = left(index);
        r = left(index);
        if(l < _heap->size && _heap->array[l].key < _heap->array[index].key)
        {
            least = l;
        }else
        {
            least = index;
        }
        if(l < _heap->size && _heap->array[r].key < _heap->array[least].key)
        {
            least = r;
        }
        if(least != index)
        {
            exchange(_heap, least, index);
            index = least;
        }else
        {
            break;
        }
    }
}

void exchange(struct heap* _heap, int i, int j)
{
    struct vertex tmp = _heap->array[i];
    _heap->array[i] = _heap->array[j];
    _heap->array[j] = tmp;
    //更新下标数组
    _heap->indexs[_heap->array[i].id] = i;
    _heap->indexs[_heap->array[j].id] = j;
}

void build_min_heap(struct heap* _heap)
{
    for(int i = _heap->size - 1; i >= 0; i--)
    {
        min_heapify(_heap, i);
    }
}

struct vertex extract_min(struct heap* _heap)
{
    //返回堆中的最小元素，然后把最后一个元素放到第一位，然后通过循环维护最小堆的性质
    struct vertex min = _heap->array[0];
    int min_vertex = _heap->array[0].id;
    _heap->indexs[min_vertex] = -1;
    _heap->array[0] = _heap->array[_heap->size - 1];
    _heap->indexs[_heap->array[0].id] = 0;      //更新下标数组
    _heap->size--;
    min_heapify(_heap, 0);
    return min;
}

void decrease_key(struct heap* _heap, struct vertex* x, int k)     //将x顶点的关键字的值减小为k
{
    int index = _heap->indexs[x->id];   //取出x顶点的下标
    _heap->array[index].key = k;
    while(index > 0 && _heap->array[parent(index)].key > _heap->array[index].key)
    {
        exchange(_heap, index, parent(index));
        index = parent(index);
    }
}

int is_empty(struct heap* _heap)
{
    if(_heap->size == 0)
    {
        return 1;
    }else
    {
        return 0;
    }
}

int exist(struct heap* _heap, struct vertex* v)
{
    if(v == NULL)
    {
        return 0;
    }else
    {
        return 1;
    }
}

struct vertex* get_node(struct heap* _heap, int v)
{
    if(_heap->indexs[v] == -1)
    {
        return NULL;        //如果顶点v不存在返回空
    }else
    {
        return &(_heap->array[_heap->indexs[v]]);       //返回Q中顶点v的那个节点的地址
    }
}
