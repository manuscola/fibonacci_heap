#include "fib_heap.h"
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

static int  fh_initheap(fib_heap* fh)
{
    assert(fh != NULL);
	fh->node_count = 0;
	fh->min = NULL;
	fh->root = NULL;
	fh->cons = NULL;
	fh->order = -1;
	return 0;
}

struct fib_heap* fh_makeheap()
{
	struct fib_heap* fh = malloc(sizeof(struct fib_heap));
	if(fh == NULL)
		return NULL;
	else
		fh_initheap(fh);

	return fh;
}

static void fh_insertafter(struct fh_node* a ,struct fh_node* b)
{
	if(a->right == a)//only one node in root list
	{
         a->right = b;
         a->left = b;
         b->right =a ;
         b->left = a;
	}
	else// great or equal 2 node in rootlist
	{
		b->right = a->right;
		a->right->left = b;
		b->left  = a;
		a->right =b;
	}
}
static void fh_insertbefore(struct fh_node* a,struct fh_node* b)
{
	fh_insertafter(a->left,b);
}
static void fh_insertrootlist(struct fib_heap* fh,struct fh_node* node)
{
	if(fh->root == NULL)
	{
		fh->root = node;
		node->left = node;
		node->right = node;
	}
	else
	{
		fh_insertafter(fh->root,node);
	}
}
static int __fh_insert(struct fib_heap* fh,struct fh_node* node)
{
	assert(fh!=NULL && node != NULL);

	fh_insertrootlist(fh,node);

	if(fh->min == NULL || fh->min->key >node->key)
	{
		fh->min = node;
	}
	fh->node_count++;
    return 0;
}
struct fh_node* fh_newnode()
{
	struct fh_node* e = malloc(sizeof(struct fh_node));
	if(e == NULL)
		return NULL;
	else
	{
		e->marked = 0;
		e->degree = 0;
		e->key = 0;
		e->data = NULL;
		e->left   = NULL;
		e->right = NULL;
		e->parent = NULL;
		e->child = NULL;
       
        return e;
	}
}

void fh_freenode(struct fh_node* node)
{
	if(node)
		free(node);
}
struct fh_node* fh_insert(struct fib_heap* fh,int key,void *data)
{
    struct fh_node* node = fh_newnode();
    if(node == NULL)
    {
         return NULL;
    }
    else
    {
    	node->data= data;
    	node->key = key;
    	__fh_insert(fh,node);
    	return node;
    }
}

static void
fh_destroyheap(struct fib_heap *h)
{

	if (h->cons != NULL)
		free(h->cons);
	h->cons = NULL;
	free(h);
}
struct fib_heap* fh_union(struct fib_heap* fh_a,struct fib_heap *fh_b)
{
	struct fh_node* lastnode_a = NULL;
	
	if(fh_a->root == NULL || fh_b->root == NULL)
	{
		if(fh_a->root == NULL)
        {
        	fh_destroyheap(fh_a);
        	return fh_b;
        }
        else
        {
        	fh_destroyheap(fh_b);
        	return fh_a;
        }
	}

    fh_a->root->left->right = fh_b->root;
    fh_b->root->left->right = fh_a->root;

    lastnode_a = fh_a->root->left;
    fh_a->root->left = fh_b->root->left;
    fh_b->root->left = lastnode_a;

    fh_a->node_count +=fh_b->node_count;
    if(fh_b->min->key < fh_a->min->key)
    	fh_a->min = fh_b->min;

    fh_destroyheap(fh_b);
    return fh_a;
}

struct fh_node* fh_remove(struct fh_node* node)
{
	struct fh_node* ret_node = NULL;
	if(node->left == node)
		ret_node = NULL;
	else
		ret_node = node->left;

	if(node->parent != NULL && node->parent->child == node)
	{
		node->parent->child = ret_node;
	}

	node->right->left = node->left;
	node->left->right = node->right;

	node->parent = NULL;
	node->left = node;
	node->right = node;

	return ret_node;

}
static void fh_removerootlist(struct fib_heap* fh,struct fh_node* node)
{
	if(node->left == node)
	{
		fh->root = NULL;
	}
	else
	{
		fh->root = fh_remove(node);
	}
}

static next_order_of_2(int size)
{
	int order = 0;
    while((1<<order)<size)
    {
        order++;
    }
    return order;
}
static int fh_checkorder(struct fib_heap* fh)
{
	int old_order ;
	if(fh->order == -1 ||fh->node_count > 1<<(fh->order))
	{
		old_order  = fh->order;
        fh->order = next_order_of_2(fh->node_count+1);
        if(fh->order < 10)
        {
        	fh->order = 10;
        }
        if(fh->order != old_order)
        {
        	struct fh_node** tmp = (struct fh_node **)realloc(fh->cons,sizeof(struct fh_node*)*(fh->order+1));
        	if(tmp == NULL)
        	{
        	    abort();
        	}
        	else
        	{
        		fh->cons = tmp;
        		return 0;
        	}
        }
      
	}
		
}

static fh_compare(struct fh_node* a,struct fh_node* b)
{
	if(a->key > b->key)
		return 1;
	else if(a->key < b->key)
		return -1;
	else
		return 0;
}
void fh_swap(struct fh_node* a,struct fh_node* b)
{
	struct fh_node * tmp = a;
	a = b;
	b = tmp;
}

static void fh_heaplink(struct fib_heap* fh,struct fh_node* big,fh_node* small)
{
    if(small->child == NULL)
    	small->child = big;
    else
    	fh_insertbefore(small->child,big);

    big->parent = small;
    small->degree++;
    big->marked = 0;
}
void  fh_consolidate(struct fib_heap* fh)
{
    int i,d,D;
    struct fh_node* w = NULL;
    struct fh_node* x = NULL;
    struct fh_node* y = NULL;
    struct fh_node** array =NULL;
    fh_checkorder(fh);

    D = fh->order +1;
    array = fh->cons;

    for(i = 0; i<D;i++)
    	array[i] = NULL;

    while((w = fh->root) != NULL)
    {
    	x = w;
    	fh_removerootlist(fh, w);
    	d= x->degree;
    	while(array[d] != NULL)
    	{
    		y = array[d];
    		if(fh_compare(x,y) >0)
    		{
    			struct fh_node* tmp = x;
    			x= y;
    			y=tmp;
    		}
    		fh_heaplink(fh,y,x);
    		array[d] = NULL;
    		d++;
    	}
    	array[d] = x;
    }

    fh->min = NULL;
    for(i = 0 ; i<D;i++)
    {
    	if(array[i] != NULL)
    	{
    		fh_insertrootlist(fh,array[i]);
    		if(fh->min == NULL || fh_compare(array[i],fh->min) < 0)
    			fh->min = array[i];
    	}
    }
 
}
struct fh_node* __fh_extract_min(struct fib_heap* fh)
{
	assert(fh != NULL);
	struct fh_node *ret_node = fh->min;
	struct fh_node* first_child = NULL;
	struct fh_node* current = NULL;
	struct fh_node* next = NULL;

	for(current = ret_node->child;current !=first_child && current!= NULL;)
	{
		if(first_child == NULL)
		{
			first_child = current;
		}
		next = current->right;
		current->parent = NULL;
		fh_insertrootlist(fh,current);
		current = next;
	}
	fh_removerootlist(fh,ret_node);
	fh->node_count--;

	if(fh->node_count == 0)
	{
		fh->min = NULL;
	}
	else
	{
		fh->min = ret_node->right;
		fh_consolidate(fh);
	}

	return ret_node;

}
void* fh_extract_min(struct fib_heap* fh)
{
	struct fh_node* node = NULL;
	void * ret = NULL;

	if(fh->min != NULL)
	{
        node = __fh_extract_min(fh);
        ret = node->data;
        fh_freenode(node);

	}
	return ret;
}

static void fh_cut(struct fib_heap*fh,struct fh_node* x,struct fh_node* y)
{
	fh_remove(x);
	y->degree--;
	fh_insertrootlist(fh, x);
	x->parent = NULL;
	x->marked = 0;
}

static void fh_cascading_cut(struct fib_heap* fh,struct fh_node* node)
{
	struct fh_node* z = node->parent;
	if(z !=NULL)
	{
		if(node->marked == 0)
		{
             node->marked = 1;
		}
		else
		{
			fh_cut(fh,node,z);
			fh_cascading_cut(fh,z);
		}
	}
}
static int fh_replacekeydata(struct fib_heap*fh, struct fh_node* node,int key,void* data)
{
	void* odata ;
	int okey;

	odata= node->data;
	okey = node->key;
    
    node->data = data;
    node->key = key;
    struct fh_node* y = node->parent;

    if(y != NULL && fh_compare(node,y) <=0)
    {
    	fh_cut(fh,node,y);
    	fh_cascading_cut(fh,y);
    }

    if(fh_compare(node,fh->min) < 0 )
    {
    	fh->min = node;
    }

    return 0;

}

int fh_replacekey(struct fib_heap* fh,struct fh_node* node,int key)
{
	int ret ;
	ret = node->key;
	if(key > node->key)
	{
		return -1;
	}
	if(key = node->key)
	{
		return 0;
	}
	return fh_replacekeydata(fh,node,key,node->data);
}

void * fh_delete(struct fib_heap* fh,struct fh_node* node)
{
	void * data = node->data;
	fh_replacekey(fh,node,INT_MIN);
	return data;
}

void fh_destroy(fib_heap* fh)
{
	while(fh->min)
		__fh_extract_min(fh);
	if(fh->cons != NULL)
		free(fh->cons);

	if(fh)
		free(fh);
}
