#include "memory.h"

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
			
			//Allocate Memory
			memptr = malloc(n_bytes);
			//*IMPORTANT: MUST ALLOCATE MORE THAN THIS
			//FOR BUDDY ALLOCATION (n_bytes+4 ?)
			
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

//Got algorithm from explainingbinary.com
int IsPowerOfTwo(long x){
	return ((x!=0) && !(x&(x-1)));
}
