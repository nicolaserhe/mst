#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

struct graph* creat_graph(char* filename)
{
    struct graph* tmp = (struct graph*)malloc(sizeof(struct graph));
    FILE *fp = fopen(filename, "r");    //定义一个文件指针，指向要操作的图文件
    if(fp == NULL)      //判断文件是否正常打开
    {
        printf("%s open failed.\n", filename);
        exit(1);
    }
    fscanf(fp, "%d %d", &(tmp->vertex_size), &(tmp->edge_size));      //读取文件第一行，分别是顶点个数和边个数
    tmp->arr = (struct e_node*)malloc(sizeof(struct e_node) * tmp->edge_size);
    int index = 0;
    for(int index = 0; index < tmp->edge_size; index++)    //读取整个文件
    {
        //将文件中的边信息存入到edge结构中
        char ch1, ch2;
        int c = fscanf(fp, "%d %c %c", &(tmp->arr[index].weight), &ch1, &ch2);
        tmp->arr[index].u = (int)(ch1 - 'a');
        tmp->arr[index].v = (int)(ch2 - 'a');
    }
    fclose(fp);
    //构建邻接表
    tmp->adj = (struct edge**)malloc(sizeof(struct edge*) * tmp->vertex_size);
    struct edge** adj = tmp->adj;
    struct e_node* arr = tmp->arr;
    struct edge* tmp_edge = NULL;
    for(int i = 0; i < tmp->vertex_size; i++)       //初始化邻接表的顶点
    {
        adj[i] = NULL;
    }
    for(int i = 0; i < tmp->edge_size; i++)         //初始化临界表的边
    {
        tmp_edge = (struct edge*)malloc(sizeof(struct edge));
        tmp_edge->v = arr[i].u;          //用边集合中的元素初始化邻接表里的边节点
        int v = arr[i].v;
        tmp_edge->weight = arr[i].weight;
        tmp_edge->next = NULL;
        if(adj[v] == NULL)         //判断当前这个顶点是否已经插入了边
        {
            adj[v] = tmp_edge;
        }else
        {
            tmp_edge->next = adj[v];
            adj[v] = tmp_edge;
        }

        tmp_edge = (struct edge*)malloc(sizeof(struct edge));
        tmp_edge->v = arr[i].v;          //用边集合中的元素初始化邻接表里的边节点
        int u = arr[i].u;
        tmp_edge->weight = arr[i].weight;
        tmp_edge->next = NULL;
        if(adj[u] == NULL)         //判断当前这个顶点是否已经插入了边
        {
            adj[u] = tmp_edge;
        }else
        {
            tmp_edge->next = adj[u];
            adj[u] = tmp_edge;
        }
    }
    return tmp;
}