#ifndef __GRAPH_H__
#define __GRAPH_H__

struct e_node       //定义一个边顶点，所有边顶点构成边集合
{
    int u;          //u和v是两个顶点编号
    int v;
    int weight;     //weight表示u和v顶点的权重
};

struct edge         //边信息
{
    int v;
    int weight;
    struct edge* next;
};

struct graph     //定义图结构
{
    struct edge** adj;      //邻接表
    int edge_size;
    int vertex_size;
    struct e_node* arr;     //边集合
};

struct graph* creat_graph(char* filename);        //初始化一个图结构
#endif
