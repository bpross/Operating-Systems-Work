#include <math.h>
#include "memory.h"
#include "buddy.h"


//Global Counter of used memory location
int MemCount = 0;

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2){
	int i;
	void *memptr;
	unsigned int f_marker = flags;
	switch(f_marker){
		case (BUDDY_ALLOC):
			//Buddy
			//printf("Buddy\n");
			
			if(!IsPowerOfTwo(n_bytes)){
				printf("Error: Buddy Allocation Invalid Size\n");
				return -1;
			}
			
            //calculate the extra bytes needed for bitmap overhead
            long chunks = n_bytes/pow(2,parm1);
            long extra_bytes = OverHeadBytes(chunks);
            printf("There will be: %lu extra bytes needed for overhead\n", extra_bytes);
			//Allocate Memory
			memptr = malloc(n_bytes+extra_bytes);
			break;
			
		case (SLAB_ALLOC):
			//Slab
			//printf("Slab\n");
			
			//Allocate Memory
			memptr = malloc(n_bytes);	
		
			break;
		case (FREE1_ALLOC):
			//Free Alloc 1
			//printf("Free1\n");
		
			//Allocate Memory
			memptr = malloc(n_bytes);
		
			break;	
		case (FREE2_ALLOC):
			//Free Alloc 2
			//printf("Free2\n");
		
			//Allocate Memory
			memptr = malloc(n_bytes);
		
			break;
		case (FREE3_ALLOC ):
			//Free Alloc 3
			//printf("Free3\n");
		
			//Allocate Memory
			memptr = malloc(n_bytes);
		
			break;
		case (FREE4_ALLOC):
			//Free Alloc 4
			//printf("Free4\n");
		
			//Allocate Memory
			memptr = malloc(n_bytes);
		
			break;
		default:
			//Undefined 
			return -1;
			break;
	}
	
	//Make sure that memptr does not contain a null location
	assert(memptr != NULL);
	
	//Make sure MemCount contains a free memory location
	while(MemAllocs[MemCount].memptr != NULL){
		MemCount++;
		if(MemCount >=512) return -1;
	}
	printf("Memcount: %d\n", MemCount);
	
	//Write information into struct
	MemAllocs[MemCount].memptr = memptr;
	MemAllocs[MemCount].n_bytes = n_bytes;
	MemAllocs[MemCount].flags = flags;
	MemAllocs[MemCount].parm1 = parm1;
	MemAllocs[MemCount].parm2 = parm2;
	MemCount++;
	
	return MemCount-1;
}


void *memalloc(int handle, long n_bytes){
    struct info * h = &MemAllocs[handle];
    printf("h:%p\n", h);
    printf("memptr: %p, n_bytes: %lu, flags: %d, parm1: %d, parm2: %p\n",
    h->memptr, h->n_bytes, h->flags, h->parm1, h->parm2);

    long alloc_bytes = n_bytes;
    printf("We want to allocate: %lu bytes\n", alloc_bytes);
    
    void *returnptr = h->memptr;

    unsigned int f_marker = h->flags;
	switch(f_marker){
		case (BUDDY_ALLOC):
			//Buddy
			printf("Buddy\n");
            //calculate the extra bytes needed for bitmap overhead

            long chunks = h->n_bytes/pow(2,h->parm1);
            long extra_bytes = OverHeadBytes(chunks);

			returnptr = buddy_alloc(h->memptr,extra_bytes,n_bytes, h->n_bytes, h->parm1);
           
           break;
			
		case (SLAB_ALLOC):
			//Slab
			printf("Slab\n");
			
			break;
		case (FREE1_ALLOC):
			//Free Alloc 1
			printf("Free1\n");
		
			break;	
		case (FREE2_ALLOC):
			//Free Alloc 2
			printf("Free2\n");
		
			break;
		case (FREE3_ALLOC ):
			//Free Alloc 3
			printf("Free3\n");
		
			break;
		case (FREE4_ALLOC):
			//Free Alloc 4
			printf("Free4\n");
		
			break;
		default:
			//Undefined 
			return NULL;
            
			break;
	}
    
    
    
    
    
    return h->memptr;
}


//Got algorithm from explainingbinary.com
int IsPowerOfTwo(long x){
	return ((x!=0) && !(x&(x-1)));
}

long OverHeadBytes(long chunks)
{
    return ceil(chunks / BYTE_SIZE);
}
