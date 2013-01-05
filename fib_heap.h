#ifndef __FIB_HEAP_H__
#define __FIB_HEAP_H__

typedef struct fh_node
{
    int marked;
    int degree;
    int key;
    void* data;
    struct fh_node* parent;
    struct fh_node* child;
    struct fh_node* left;
    struct fh_node* right;
}fh_node;

typedef struct fib_heap
{
    int node_count;
    int order;
    struct fh_node* min;
    struct fh_node* root;
    struct fh_node** cons;
}fib_heap;


struct fib_heap* fh_makeheap();

struct fh_node* fh_insert(struct fib_heap* fh,int key,void *data);
struct fib_heap* fh_union(struct fib_heap* fh_a,struct fib_heap *fh_b);

void* fh_extract_min(struct fib_heap* fh);
int fh_replacekey(struct fib_heap* fh,struct fh_node* node,int key);
void* fh_delete(struct fib_heap* fh,struct fh_node* node);
void fh_destroy(fib_heap* fh);

#endif