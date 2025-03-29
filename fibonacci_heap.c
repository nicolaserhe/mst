#include "fibonacci_heap.h"

//TODO:创建堆
struct heap* make_heap()
{
    struct heap* tmp = (struct heap*)malloc(sizeof(struct heap));
    tmp->nil = (struct node*)malloc(sizeof(struct node));
    tmp->nil->key = INT_MAX;        //这样设置，在插入的时候就不需要判断斐波那契堆是否为空了
    tmp->nil->left = tmp->nil;
    tmp->nil->right = tmp->nil;
    tmp->nil->degree = INT_MAX;     //这样设置防止空节点与其他节点合并
    tmp->min = tmp->nil;
    tmp->size = 0;
    return tmp;
}

struct heap* creat_heap(struct graph* _graph)
{
    struct heap* tmp = make_heap();
    tmp->flag = (int*)malloc(sizeof(int) * _graph->vertex_size);
    tmp->address = (struct node**)malloc(sizeof(struct node*) * _graph->vertex_size);
    for(int i = 0; i < _graph->vertex_size; i++)        //将图的顶点存入斐波那契堆
    {
        insert(tmp, alloc_node(tmp, i));        //将顶点包装后插入到斐波那契堆中
    }
    tmp->min->key = 0;
    tmp->capacity = tmp->size;
    return tmp;
}

struct node* alloc_node(struct heap* _heap, int vertex)         //包装节点
{
    struct node* tmp = (struct node*)malloc(sizeof(struct node));
    tmp->id = vertex;
    tmp->key = INT_MAX - 1;     //保证nil节点的key最大，保证当堆中有元素时，nil节点一定不会被min指向
    tmp->parent = -1;
    _heap->flag[vertex] = 1;        //标记顶点，设置当前顶点的标记位位1表示顶点在斐波那契堆里
    _heap->address[vertex] = tmp;   //将包装后的顶点的地址存储在address数组中，方便寻找
    return tmp;
}

struct node* get_node(struct heap* _heap, int v)
{
    return _heap->address[v];
}

int exist(struct heap* _heap, struct node* v)
{
    if(_heap->flag[v->id] == 0)
    {
        return 0;
    }else
    {
        return 1;
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

void decrease_key(struct heap* h, struct node* x, int k)
{
    heap_decrease_key(h, x, k);
}




//TODO：插入节点
void insert(struct heap* h, struct node* x)
{
    //初始化节点的各个元素
    x->degree = 0;
    x->p = NULL;
    x->child = NULL;
    x->mark = FALSE;

    //将x插入斐波那契堆的根链表中
    x->right = h->min->right;
    x->left = h->min;
    x->right->left = x;
    x->left->right = x;

    //更新斐波那契堆的堆顶
    if(x->key < h->min->key)
    {
        h->min = x;
    }
    h->size = h->size + 1;
}

//TODO：返回最小值
struct node* minimum(struct heap* h)
{
    return h->min;
}

//TODO：合并斐波那契堆
struct heap* heap_union(struct heap* h1, struct heap* h2)
{
    struct heap* h = make_heap();
    h->min = h1->min;
    //连接h1和h2的根链表然后将这个根链表赋值赋值给h
    if(h2->min == NULL)
    {
        h->min = h1->min;
    }else if(h1->min == NULL)
    {
        h->min = h2->min;
    }else
    {
        struct node* head = h2->min;
        struct node* tail = h2->min->left;
        head->left = h1->min;
        tail->right = h1->min->right;
        head->left->right = head;
        tail->right->left = tail;
        if(h2->min->key < h1->min->key)
        {
            h->min = h2->min;
        }
    }
    h->size = h1->size + h2->size;
    return h;
}

//TODO：抽取最小节点
struct node* extract_min(struct heap* h)
{
    struct node* z = h->min;
    //将z的孩子链接到根链表,并将z从根链表中删除
    if(z->child != NULL)        //如果z的孩子不为空
    {
        struct node* head = z->child;
        struct node* tail = head->left;
        struct node* index = head;

        for(int i = 0; i < z->degree; i++)
        {
            index->p = NULL;
            index = index->right;
        }
        head->left = z->left;
        tail->right = z->right;
        head->left->right = head;
        tail->right->left = tail;
    }else
    {
        //如果z没有孩子直接把z从根节点剔除
        z->left->right = z->right;
        z->right->left = z->left;
    }
    h->size = h->size - 1;
    h->flag[z->id] = 0;
    if(h->size != 0)     //如果斐波那契堆还存在元素
    {
        h->min = h->nil->right;
        consolidate(h);             //否则的话调用consolidate函数将堆归并
    }else
    {
        h->min = h->nil;
    }

    return z;
}

void consolidate(struct heap* h)
{
    int max_degree = fibonacci_heap_degree(h->size);     //计算堆的最大度

    //定义一个辅助数组。然后置空
    struct node** tmp = (struct node**)malloc(sizeof(struct node*) * max_degree);
    for(int i = 0; i < max_degree; i++)
    {
        tmp[i] = NULL;
    }
    //遍历斐波那契堆的根链表，合并相同度的链表
    h->nil->right->left = h->nil->left;     //先将空节点剔除
    h->nil->left->right = h->nil->right;
    struct node* head = h->min;
    struct node* tail = h->min->left;
    head->left = NULL;
    tail->right = NULL;
    struct node* x = NULL;
    struct node* y = NULL;
    struct node* w = head;
    //对根链表上的每个节点进行归并
    while(w != NULL)
    {
        x = w;
        w = w->right;
        int d = x->degree;
        //如果当前节点和辅助数组里的节点度数相同，那么合并两节点，将x更新为合并后节点
        while(tmp[d] != NULL)
        {
            y = tmp[d];
            //如果x的关键字大于y的v关键字，那么交换x和y
            if (x->key > y->key)
            {
                struct node* t = x;
                x = y;
                y = t;
            }
            heap_link(h, y, x);     //将y节点接入到x的孩子中
            tmp[d] = NULL;          //将度为d的哪个临时数组置为空，然后递增d，以此循环，直到临时数组没有节点为止
            d = d + 1;
        }
        tmp[d] = x;     //当临时数组中没有和x有相同度的节点时，将x存入度为d的临时数组中
    }
    h->min = NULL;      //将斐波那契堆的最小值指向NULL，因为目前不知道最小值是谁
    //将临时数组中的节点链接起来
    h->nil->right = h->nil;
    h->nil->left = h->nil;

    //将根链表串联起来
    for(int i = 0; i < max_degree; i++)
    {
        if(tmp[i] != NULL)
        {
            tmp[i]->right = h->nil->right;
            tmp[i]->left = h->nil;
            tmp[i]->right->left = tmp[i];
            tmp[i]->left->right = tmp[i];
        }
    }
    //找寻关键字最小的节点
    h->min = h->nil->right;
    struct node* index = h->min->right;
    while(index != h->nil)
    {
        if(index->key < h->min->key)
        {
            h->min = index;
        }
        index = index->right;
    }
}

int fibonacci_heap_degree(int n)
{
    double gs = 1.618;
    double d = log(n) / log(gs);
    return (int)d + 1;
}

void heap_link(struct heap* h, struct node* y, struct node* x)
{
    if(x->child != NULL)
    {
        //如果x有孩子，那么将y插入到x的孩子中去
        struct node* before = x->child;
        struct node* after = before->right;
        y->left = before;
        y->right = after;
        before->right = y;
        after->left = y;
        y->p = x;
    }else
    {
        x->child = y;
        y->right = y;
        y->left = y;
        y->p = x;
    }
    x->degree = x->degree + 1;      //递增x的度
    y->mark = FALSE;            //清除y的标记，因为y已经被移除
}

//TODO:减少某个节点的关键字
void heap_decrease_key(struct heap* h, struct node* x, int k)
{
    if(k > x->key)
    {
        printf("new key is greater than current key.\n" );
        exit(1);
    }
    x->key = k;
    struct node* y = x->p;
    if(y != NULL && x->key < y->key)        //如果x的父节点不是空，且x的关键字比y的关键字小，那么需要进行切断
    {
        cut(h, x, y);       //将x从y上切断
        cascading_cut(h, y);        //判断y是否需要切断，如果需要那么进行级联切断
    }
    if(x->key < h->min->key)
    {
        h->min = x;
    }
}

void cut(struct heap* h, struct node* x, struct node* y)
{
    //将x从y上切断
    if(x->right == x)       //如果y的孩子只有x节点那么切断x的时候直接将y的child指向空
    {
        y->child = NULL;

    }else
    {
        if(y->child == x)       //如果y的孩子节点指向的是x，那么将y的孩子节点指向x的下一个节点
        {
            y->child = x->right;
        }
        struct node* before = x->left;
        struct node* after = x->right;
        before->right = after;
        after->left = before;

    }
    y->degree = y->degree - 1;      //更新y的度
    //将x链接到根链表中
    x->right = h->min->right;
    x->left = h->min;
    x->right->left = x;
    x->left->right = x;
    x->p = NULL;
    x->mark = FALSE;
}
void cascading_cut(struct heap* h, struct node* y)
{
    struct node* z = y->p;
    if(z != NULL)
    {
        if(y->mark == FALSE)        //如果y没有被标记说明y没有失去过孩子，不需要进行级联切点
        {
            y->mark = TRUE;
        }else       //如果y失去过一个孩子了，那么这次失去第二个孩子的时候需要将y进行级联切断
        {
            cut(h, y, z);
            cascading_cut(h, z);
        }
    }
}
