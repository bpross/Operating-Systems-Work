#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

int main(int argc, char**argv){
	
	int parm1 = 12;
	int parm2 = 1;
	long size = 524288;
	
	//Initialize memory
	int h1 = meminit(size,BUDDY_ALLOC,parm1,&parm2);
	//int h2 = meminit(size,SLAB_ALLOC,parm1,&parm2);
	//int h3 = meminit(size,FREE1_ALLOC,parm1,&parm2);
	//int h4 = meminit(size,FREE2_ALLOC,parm1,&parm2);
	//int h5 = meminit(size,FREE3_ALLOC,parm1,&parm2);
	//int h6 = meminit(size,FREE4_ALLOC,parm1,&parm2);
	
	//this one should fail due to size
	//int h7 = meminit(511, BUDDY_ALLOC, parm1, &parm2);
	
	//Debug Prints
	int i;
	//printf("%d, %d, %d, %d, %d, %d\n", h1, h2, h3, h4, h5, h6);
	for(i=0;i<1;i++){
		struct info * h = &MemAllocs[i];
		printf("h:%p\n", h);
		printf("memptr: %p, n_bytes: %lu, flags: %d, parm1: %d, parm2: %p\n",
			h->memptr, h->n_bytes, h->flags, h->parm1, h->parm2);
	}

    void *testptr = memalloc(h1,512);
    printf("memptr: %p\n", testptr);
	//Free Memory
	free(MemAllocs[h1].memptr);
	//free(MemAllocs[h2].memptr);
	//free(MemAllocs[h3].memptr);
	//free(MemAllocs[h4].memptr);
	//free(MemAllocs[h5].memptr);
	//free(MemAllocs[h6].memptr);
	return 0;
}
