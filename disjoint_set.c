#include <stdio.h>
#include <stdlib.h>
#include "disjoint_set.h"

struct set* creat_set()
{
    struct set* tmp = (struct set*)malloc(sizeof(struct set));
    tmp->array = (struct set_node*)malloc(sizeof(struct set_node) * MAX_SIZE);
    tmp->size = 0;
    return tmp;
}

void make_set(struct set* _set, int x)
{
    _set->array[_set->size].p = _set->size;      //将x的父亲设置为自己
    _set->array[_set->size].rank = 0;  //将x的秩设为0
    _set->size++;
}

int find_set(struct set* _set, int x)
{
    struct set_node* array = _set->array;
    int representative = x;     //代表将来会存入representative中
    while(array[representative].p != representative)  //找到当前不相交集合的代表
    {
        representative = _set->array[representative].p;
    }
    while(x != representative)        //找到之后将沿途的元素的父下标设置为代表
    {
        int tmp = array[x].p;
        array[x].p = representative;
        x = array[x].p;
    }
    return representative;       //返回代表
}

void union_set(struct set* _set, int x, int y)
{
    link(_set, find_set(_set, x), find_set(_set, y));   //将x和y链接起来
}

void link(struct set* _set, int x, int y)
/*
**如果x的秩大于y的秩，那么y接入到x中，否则x接入到y中，
**如果x和y的秩相等的话，那么将其中一个接入到另一个中，作为父顶点的那个顶点秩加1
*/
{
    struct set_node* arr = _set->array;

    if(arr[x].rank > arr[y].rank)
    {
        arr[y].p = x;
    }else
    {
        arr[x].p = y;
        if(arr[x].rank == arr[y].rank)
        {
            arr[y].rank++;
        }
    }
}
