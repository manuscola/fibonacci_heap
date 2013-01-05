#include <stdio.h>
#include <stdlib.h>
#include "fib_heap.h"


int fh_test()
{
	struct fib_heap*  fh = fh_makeheap();
	if(fh == NULL)
	{
		fprintf(stderr, "malloc fh failed\n");
		return -1;
	}
    int * array = malloc(sizeof(int)*1024);
    if(array == NULL)
    {
    	fprintf(stderr, "malloc array failed");
    	return -2;
    }

    int i ; 
    for(i = 0; i <10 ;i++)
    {
    	array[i] = rand()%4096;
    	fprintf(stdout,"insert %d\n",array[i]);
    	fh_insert(fh,array[i],&array[i]);
    }

    for(i = 0 ; i < 6;i++)
    {
    	int *p = (int*)fh_extract_min(fh);
    	fprintf(stdout,"extract %d\n",*p);
    }


    struct fib_heap* fh_b = fh_makeheap();
    int* array_2= malloc(sizeof(int)* 1024);
    for(i = 0; i<10;i++)
    {
        array_2[i] = rand()%100;
        fprintf(stdout,"insert %d\n",array_2[i]);
        fh_insert(fh_b,array_2[i],&array_2[i]);
    }
    fh = fh_union(fh,fh_b);
    for(i = 0 ; i < 6;i++)
    {
        int *p = (int*)fh_extract_min(fh);
        fprintf(stdout,"extract %d\n",*p);
    }

    fh_destroy(fh);
    return 0;

}
int main()
{
	return fh_test();
}
