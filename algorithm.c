#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algorithm.h"
void quick_sort(struct e_node* _edge, int p, int r)
{
    while(p < r)
    {
        int q = partition(_edge, p, r);
        int left = q - p;
        int right = r - q;
        if(left < right)    //减少递归调用的栈深度，选择元素个数少的那边进行递归，将栈深度限制在O(lgn)中
        {
            quick_sort(_edge, p, q - 1);
            p = q + 1;
        }else
        {
            quick_sort(_edge, q + 1, r);
            r = q - 1;
        }
    }
}

int partition(struct e_node* _edge, int p, int r)
{
    srand(time(NULL));  //初始化随机数种子
    int index = rand() % (r - p + 1);   //生成一个随机数范围是0-子数组长度 - 1
    index = index + p;      //index为随机后主元的下标
    struct e_node tmp = _edge[r];   //将主元和子数组的最后一个元素进行交换
    _edge[r] = _edge[index];
    _edge[index] = tmp;
    struct e_node x = _edge[r];   //x为主元
    int i = p - 1;

    /**
     * 遍历子数组，i是定义的已经目前已经遍历过的子数组元素中比主元小的元素中最大的下标，
     * 因此i之前的元素满足快速排序的循环不变式
     * 因为最后一个元素是主元，所以不能遍历最后一个元素
     * i及其之前的元素小于主元，（i+1，j）之间的元素大于主元，当j=r的时候，
     * 子数组中i及其之前的元素都小于主元，（i+1，r-1）之间的元素大于主元
    */
    for(int j = p; j < r; j++)
    {
        if(_edge[j].weight < x.weight)      //如果当前下标小于主元，那么i指向的元素的下一个进行交换
        {
            i = i + 1;
            tmp = _edge[j];
            _edge[j] = _edge[i];
            _edge[i] = tmp;
        }
    }
    //将最后一个元素和下标为i+1的元素进行交换，i+1位置即为主元，主元之前的元素都比主元小，主元之后的元素都比主元大
    tmp = _edge[i + 1];
    _edge[i + 1] = _edge[r];
    _edge[r] = tmp;
    return i + 1;   //返回主元下标
}

struct mst* mst_Kruskal(struct graph* _graph)
{
    //因为最小生成树的边数为顶点数减1，所以我们只需要初始化顶点数减1数量的空间来存放最小生成树就行
    struct e_node* mst = (struct e_node*)malloc(sizeof(struct e_node) * (_graph->vertex_size - 1));
    struct set* mst_set = creat_set();      //创建一个不相交集合结构（一个不相交集合结构中可以有多个不相交集合）
    for(int i = 0; i < _graph->vertex_size; i++)    //遍历顶点，为每个顶点创建一个不相交集合
    {
        make_set(mst_set, i);
    }
    quick_sort(_graph->arr, 0, _graph->edge_size - 1);

    int index = 0;
    for(int i = 0; i < _graph->edge_size; i++)
    {
        //如果两个顶点的代表不一样说明这两个顶点不在一个集合即两个顶点之间目前还没有路径，那么当前边是安全的
        if(find_set(mst_set, _graph->arr[i].u) != find_set(mst_set, _graph->arr[i].v))
        {
            mst[index] = _graph->arr[i];     //将这条安全边压入mst中
            index++;
            //如果当前两个顶点不在一个不相交集合，那么合并两个顶点的集合
            union_set(mst_set, _graph->arr[i].u,  _graph->arr[i].v);
        }
    }
    struct mst* _mst = (struct mst*)malloc(sizeof(struct mst));
    _mst->data = (void*)mst;
    _mst->size = _graph->vertex_size - 1;
    _mst->flag = 0;
    return _mst;
}

struct mst* mst_Prim(struct graph* _graph)
{
    struct heap* prim_heap = creat_heap(_graph);        //创建堆
    struct vertex* save = (struct vertex*)malloc(sizeof(struct vertex) * _graph->vertex_size);
    int save_index = 0;
    while(!is_empty(prim_heap))
    {


#ifdef __BINARY_HEAP_H__
        save[save_index] = extract_min(prim_heap);     //将最小堆中关键字最小的元素弹出
#else
        struct node* tmp = extract_min(prim_heap);      //将斐波那契堆中的关键字最小的元素弹出
        //把节点信息保存到save数组中
        save[save_index].id = tmp->id;
        save[save_index].key = tmp->key;
        save[save_index].parent = tmp->parent;
        prim_heap->flag[tmp->id] = 0;       //因为弹出最小元素，所以将弹出的这个顶点标记位置0
#endif


        int u = save[save_index].id;
        save_index++;
        //遍历顶点u的所有边
        struct edge* index = _graph->adj[u];


        //因为二叉堆和斐波那契堆的节点类型不同所以要条件编译
#ifdef __BINARY_HEAP_H__
            struct vertex* v;
#else
            struct node* v;
#endif


        while(index != NULL)
        {
            v = get_node(prim_heap, index->v);      //包装节点
            if(exist(prim_heap, v) && index->weight < v->key)
            {
                v->parent = u;
                decrease_key(prim_heap, v, index->weight);
            }
            index = index->next;
        }
    }
    struct mst* _mst = (struct mst*)malloc(sizeof(struct mst));
    _mst->data = (void*)save;


#ifdef __BINARY_HEAP_H__
    _mst->size = _graph->vertex_size = 1;
#else
    _mst->size = _graph->vertex_size;
#endif


    _mst->flag = 1;
    return _mst;
}

void save_mst(struct mst* _mst, char* filename)
{
    if(_mst->flag == 0)     //如果是struct e_node结构的最小生成树输出算法如下
    {
        struct e_node* _edge = (struct e_node*)(_mst->data);
        FILE* fp = fopen(filename, "w");
        if(fp == NULL)
        {
            printf("%s open failed.\n", filename);
            exit(1);
        }
        for(int i = 0; i < _mst->size; i++)
        {
            fprintf(fp, "%d %c %c\n", _edge[i].weight, 'a' + _edge[i].u, 'a' + _edge[i].v);
        }
        fclose(fp);
    }else
    {
        struct vertex* mst = (struct vertex*)(_mst->data);
        FILE* fp = fopen(filename, "w");
        if(fp == NULL)
        {
            printf("%s open failed.\n", filename);
            exit(1);
        }
        for(int i = 1; i < _mst->size; i++)
        {
            fprintf(fp, "%d %c %c\n", mst[i].key, mst[i].parent + 'a', mst[i].id + 'a');
        }
        fclose(fp);
    }
}
