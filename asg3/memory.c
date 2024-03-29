/* This is assignment 3 for CMPS111 at UCSC Spring 2012
 *  * Authors: Benjamin Ross, David Lau, Vincent Wong
 *   */

#include "memory.h"
#include "buddy.h"
#include "free_allocation.h"


//Global Counter of used memory location
int MemCount = 0;

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2){
	int i;
	void *memptr = NULL;
	unsigned int f_marker = flags;
	long overhead=0;
	
	switch(f_marker){
		case (BUDDY_ALLOC):
			//Buddy
			printf("Buddy Alloc\n");
			if(!IsPowerOfTwo(n_bytes)){
				printf("Error: Buddy Allocation Invalid Size\n");
				return -1;
			}
			if(parm1 < 0 || parm1 >30){
				printf("Error: Buddy Allocation parm1 is invalid\n");
				return -1;
			}
			if((long)power(2,parm1) > n_bytes){
				printf("Error: Buddy Minimum page larger than allocated size\n");
				return -1;
			}
			int levels = calculate_levels(n_bytes, (long)power(2,parm1));
			printf("Buddy Init: Levels: %d\n", levels);
			
			overhead = calculate_overhead(n_bytes, parm1);
			printf("Buddy Init: Calculated overhead: %lu byte(s)\n", overhead);
			printf("Buddy Alloc: Allocating %lu bytes, Actual %lu bytes\n", n_bytes, n_bytes+overhead);
			
			memptr = malloc(n_bytes+overhead);
			initialize_bitmap(memptr,overhead);
			memptr += overhead;
			break;
			
		case (SLAB_ALLOC):
			//Slab
			//printf("Slab\n");
			
			//Allocate Memory
			memptr = malloc(n_bytes);	
		
			break;
		case (FREE1_ALLOC):
			//Free Alloc 1
			printf("Free Alloc: Allocating %lu bytes, Actual: %lu\n",n_bytes, n_bytes+4);
			//Allocate Memory
			memptr = malloc(n_bytes+4);
			//store size
			*(long *)memptr = n_bytes;
			memptr+=4;
		
			break;	
		case (FREE2_ALLOC):
			//Free Alloc 2
			printf("Free Alloc: Allocating %lu bytes, Actual: %lu\n",n_bytes, n_bytes+4);
			
			//Allocate Memory
			memptr = malloc(n_bytes+4);
			//store size
			*(long *)memptr = n_bytes;
			memptr+=4;
		
			break;
		case (FREE3_ALLOC ):
			//Free Alloc 3
			printf("Free Alloc: Allocating %lu bytes, Actual: %lu\n",n_bytes, n_bytes+4);
			
		
			//Allocate Memory
			memptr = malloc(n_bytes+4);
			//store size
			*(long *)memptr = n_bytes;
			memptr+=4;
			break;
		case (FREE4_ALLOC):
			//Free Alloc 4
			printf("Free Alloc: Allocating %lu bytes, Actual: %lu\n",n_bytes, n_bytes+4);
		
			//Allocate Memory
			memptr = malloc(n_bytes+4);
			//store size
			*(long *)memptr = n_bytes;
			memptr+=4;
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
	//printf("Memcount: %d\n", MemCount);
	
	//Write information into struct
	MemAllocs[MemCount].memptr = memptr;
	MemAllocs[MemCount].n_bytes = n_bytes;
	MemAllocs[MemCount].flags = flags;
	MemAllocs[MemCount].parm1 = parm1;
	MemAllocs[MemCount].parm2 = parm2;
	
	//If free list, insert these as well
	if(f_marker == FREE1_ALLOC || f_marker == FREE2_ALLOC 
		|| f_marker == FREE3_ALLOC  || f_marker == FREE4_ALLOC ){
			MemAllocs[MemCount].free_list = memptr;
			*((long **)MemAllocs[MemCount].free_list) = NULL;
			MemAllocs[MemCount].last_alloc = NULL;
		}
	
	//If Buddy Allocation, Insert these as well
	if(f_marker == BUDDY_ALLOC){
		//printf("Storing Overhead: %lu\n", overhead);
		MemAllocs[MemCount].overhead = overhead;
	}
	
	MemCount++;
	
	return MemCount-1;
}


void *memalloc(int handle, long n_bytes){
    struct info * h = &MemAllocs[handle];

    long alloc_bytes = n_bytes;
    if(alloc_bytes == 0){
      printf("memalloc error: invalid alloc size\n");
      return NULL;
    }
    printf("Attempting to allocate: %lu bytes\n", alloc_bytes);
    
    void *returnptr = h->memptr;

    unsigned int f_marker = h->flags;
	switch(f_marker){
		case (BUDDY_ALLOC):
			//Buddy
			printf("Buddy Alloc:\n");
            		returnptr = buddy_allocation(h, n_bytes);
           		break;
			
		case (SLAB_ALLOC):
			//Slab
			printf("Slab\n");
			
			break;
		case (FREE1_ALLOC):
			//Free Alloc 1
			printf("Free Alloc: First Fit\n");
			returnptr = free_allocation(h, n_bytes);
			if(returnptr!=NULL)
				h->last_alloc = returnptr;
			break;	
		case (FREE2_ALLOC):
			//Free Alloc 2
			printf("Free Alloc: Next Fit\n");
			returnptr = free_allocation(h, n_bytes);
			if(returnptr!=NULL)
				h->last_alloc = returnptr;
			break;
		case (FREE3_ALLOC ):
			//Free Alloc 3
			printf("Free Alloc: Best Fit\n");
			returnptr = free_allocation(h, n_bytes);
			if(returnptr!=NULL)
				h->last_alloc = returnptr;
			break;
		case (FREE4_ALLOC):
			//Free Alloc 4
			printf("Free Alloc: Worst Fit\n");
			returnptr = free_allocation(h, n_bytes);
			if(returnptr!=NULL)
				h->last_alloc = returnptr;
			break;
		default:
			//Undefined 
			returnptr = NULL;
            
			break;
	}
    
    
    
    
    if(returnptr == NULL) printf("Could not find enough space\n");
    return returnptr;
}

void memfree (void *region){
	if(region == NULL){
		printf("memfree error: region is NULL\n");
		return;
	}
	int handle = find_handle(region);
	if(handle == -1){
		printf("memfree: Could not find region handle\n");
		return;
	}
	//printf("Found Handle: %d\n", handle);
	switch(MemAllocs[handle].flags){
		case BUDDY_ALLOC:
			printf("memfree: Buddy Type\n");
			buddy_free(&MemAllocs[handle], region);
			break;
		case SLAB_ALLOC:
			printf("memfree: Slab Type\n");
			break;
		case FREE1_ALLOC:
			printf("memfree: Free List Type\n");
			free_list_free(handle, region);
			break;
		case FREE2_ALLOC:
			printf("memfree: Free List Type\n");
			free_list_free(handle, region);
			break;
		case FREE3_ALLOC:
			printf("memfree: Free List Type\n");
			free_list_free(handle, region);
			break;
		case FREE4_ALLOC:
			printf("memfree: Free List Type\n");
			free_list_free(handle, region);
			break;
		default:
			printf("Could not find handle\n");
			break;
	}
}

int find_handle(void *region){
	int i=0;
	while(MemAllocs[i].memptr != NULL){
		if(region >= MemAllocs[i].memptr &&
			region < (MemAllocs[i].memptr+MemAllocs[i].n_bytes)){
			return i;
		}
		i++;
		if(i>=512)break;
	}
}

//Got algorithm from explainingbinary.com
int IsPowerOfTwo(long x){
	return ((x!=0) && !(x&(x-1)));
}

void free_allocs(int handle){
	switch(MemAllocs[handle].flags){
		case BUDDY_ALLOC:
			free(MemAllocs[handle].memptr - MemAllocs[handle].overhead);
			break;
		case SLAB_ALLOC:
			free(MemAllocs[handle].memptr);
			break;
		case FREE1_ALLOC:
			free(MemAllocs[handle].memptr-4);
			break;
		case FREE2_ALLOC:
			free(MemAllocs[handle].memptr-4);
			break;
		case FREE3_ALLOC:
			free(MemAllocs[handle].memptr-4);
			break;
		case FREE4_ALLOC:
			free(MemAllocs[handle].memptr-4);
			break;
		default:
			printf("Could not find handle\n");
			break;
	}
	
}


