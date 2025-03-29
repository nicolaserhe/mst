#ifndef __DISJOINT_SET_H__
#define __DISJOINT_SET_H__

#define MAX_SIZE 100

struct set_node     //定义不相交的元素
{
    int p;          //定义父顶点的下标
    int rank;       //定义顶点的秩
};

struct set
{
    struct set_node* array;        //不相交集合的数组，所有不相交集合的元素都在这个数组里
    int size;                   //数组的大小
};

struct set* creat_set();        //创建不相交集合结构
void make_set(struct set* _set, int x);             //创建不相交集合元素
int find_set(struct set* _set, int x);              //找寻当前节点的代表
void union_set(struct set* _set, int x, int y);     //合并两个不相交集合的元素
void link(struct set* _set, int x, int y);          //union_set函数会调用当前函数实现链接操作
#endif