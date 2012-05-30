/* This is assignment 3 for CMPS111 at UCSC Spring 2012
 *  * Authors: Benjamin Ross, David Lau, Vincent Wong
 *   */

#include "memory.h"
#include "free_allocation.h"

void *free_allocation(info_p handler, long n_bytes){
	int full_mem=0;
	
	if(n_bytes < 0){
		printf("Free Allocation Error: requested negative space\n");
		return NULL;
	}
	
	if(handler->n_bytes < n_bytes){
		printf("Free Allocation Error: requested too much space\n");
		return NULL; //User requested more space than permitted
	}
	
	if(n_bytes == handler->n_bytes){
		//printf("request:%lu; bytes: %lu; declaring full memory\n", n_bytes, handler->n_bytes);
		full_mem=1;
	}
	
	if(handler->free_list == NULL){
		return NULL;
	}
	
	void *mem = handler->memptr-4;
	//printf("free list head: %p; size of mem is: %lu\n",handler->free_list, *(long *)mem);
	
	switch(handler->flags){
		case (FREE1_ALLOC): //first fit
			return first_fit(handler, handler->free_list, n_bytes,full_mem);
			break;
		case (FREE2_ALLOC): //next fit
			return next_fit(handler, n_bytes);
			break;
		case (FREE3_ALLOC): //best fit
			return best_fit(handler, n_bytes);
			break;
		case (FREE4_ALLOC): //worst fit
			return worst_fit(handler, n_bytes);
			break;
		default: //should never happen
			return NULL;
	}
	
	printf("Not enough memory to allocate\n");
	return NULL;
}

void *first_fit(info_p handler, void *free_list, long n_bytes, int flag){
	void *look_through = handler->free_list;
	void *prev = NULL;
	//look through free list for new allocations
	do{
		void *sizer = look_through-4; //size of free space
		printf("Check:Block Size:%lu byte(s), Allocating: %lu byte(s)\n",*(long *)sizer, n_bytes);
		if(*(long *)sizer >= n_bytes){
			//edit free list, insert size in first four bytes, and return pointers
			if(*(long *)sizer == n_bytes) //exact fit (good)
				return allocate_to_list_exact(handler, free_list, look_through, n_bytes, prev);
			
			if((*(long *)sizer - n_bytes) >= 8) //Leaves block large enough for 4 byte size + 4 byte data
				return allocate_to_list(handler, free_list, look_through, n_bytes, prev);
			//Otherwise, the chunk does not work!
		}
		//printf("Look Through Next: %p\n", *(long **)look_through );
		if(*(long **)look_through == NULL) break; //last node of list
		prev = look_through; //keep track of previous node
		look_through = *(long **)look_through; //look through = next spot
	}while(look_through != NULL);
	
	return NULL;
}

void *next_fit(info_p handler, long n_bytes){
	void *curr = handler->free_list;
	void *prev = NULL;
	printf("Last Allocation: %p\n", handler->last_alloc);
	if(handler->last_alloc == NULL){
		return first_fit(handler, handler->free_list, n_bytes, 0);
	}
	while(curr < handler->last_alloc){
		prev = curr;
		curr = *(long **)curr;
	} //Look for next available free space after previous allocated region
	while(curr != NULL){
		if(free_size(curr) >= n_bytes){
			//edit free list, insert size in first four bytes, and return pointers
			if(free_size(curr) == n_bytes) //exact fit (good)
				return allocate_to_list_exact(handler, handler->free_list, curr, n_bytes, prev);
			
			if((free_size(curr)  - n_bytes) >= 8) //Leaves block large enough for 4 byte size + 4 byte data
				return allocate_to_list(handler, handler->free_list, curr, n_bytes, prev);
				//Otherwise, the chunk does not work!
		}
		prev = curr; //keep track of previous node
		curr = *(long **)curr;
	}//hit the end of list, go back to beginning
	prev = NULL;
	curr = handler->free_list;
	while(curr < handler->last_alloc){
		if(free_size(curr) >= n_bytes){
			//edit free list, insert size in first four bytes, and return pointers
			if(free_size(curr) == n_bytes) //exact fit (good)
				return allocate_to_list_exact(handler, handler->free_list, curr, n_bytes, prev);
			
			if((free_size(curr)  - n_bytes) >= 8) //Leaves block large enough for 4 byte size + 4 byte data
				return allocate_to_list(handler, handler->free_list, curr, n_bytes, prev);
				//Otherwise, the chunk does not work!
		}
		prev = curr; //keep track of previous node
		curr = *(long **)curr;
	}
	return NULL;
}

void *best_fit(info_p handler, long n_bytes){
	void *best = NULL;
	void *prev = NULL;
	void *bestprev = NULL;
	void *curr = handler->free_list;
	while(curr != NULL){
		if(best == NULL){
			if(free_size(curr) > n_bytes){
				bestprev = prev;
				best = curr;
				//printf("best size: %lu\n", free_size(best));
			}
		}
		if(free_size(curr) < free_size(best) && free_size(curr) >= n_bytes){
			bestprev = prev;
			best = curr;
			//printf("best size: %lu\n", free_size(best));
		}
		prev = curr;
		curr = *(long **)curr;
	}
	printf("Best Size: %lu at %p\n", free_size(best), best);
	//Enter Best Size
	if(free_size(best) == n_bytes) //exact fit (good)
		return allocate_to_list_exact(handler, handler->free_list, best, n_bytes, bestprev);
			
	if((free_size(best)  - n_bytes) >= 8) //Leaves block large enough for 4 byte size + 4 byte data
		return allocate_to_list(handler, handler->free_list, best, n_bytes, bestprev);
	//Otherwise, the chunk does not work!
	
	return NULL;
}

void *worst_fit(info_p handler, long n_bytes){
	void *worst = NULL;
	void *prev = NULL;
	void *worstprev = NULL;
	void *curr = handler->free_list;
	while(curr != NULL){
		if(worst == NULL){
			if(free_size(curr) > n_bytes){
				worstprev = prev;
				worst = curr;
				//printf("worst size: %lu\n", free_size(worst));
			}
		}
		if(free_size(curr) > free_size(worst) && free_size(curr) >= n_bytes){
			worstprev = prev;
			worst = curr;
			//printf("worst size: %lu\n", free_size(worst));
		}
		prev = curr;
		curr = *(long **)curr;
	}
	printf("Worst Size: %lu at %p\n", free_size(worst), worst);
	//Enter Best Size
	if(free_size(worst) == n_bytes) //exact fit (good)
		return allocate_to_list_exact(handler, handler->free_list, worst, n_bytes, worstprev);
			
	if((free_size(worst)  - n_bytes) >= 8) //Leaves block large enough for 4 byte size + 4 byte data
		return allocate_to_list(handler, handler->free_list, worst, n_bytes, worstprev);
	//Otherwise, the chunk does not work!
	
	
	return NULL;
}

long free_size(void *region){
	return *(long *)(region-4);
}

void *allocate_to_list(info_p handler, void *free_list, void *look_through, long n_bytes, void *prev){
	//Linked list with pointer arithmetic
	//printf("free:%p; look_through: %p\n",free_list,look_through);
	if(look_through == free_list){ //head of list
		//printf("Head of list\n");
		void *sizer = look_through-4; //modify the size of the list
		long new_size = *(long *)sizer - n_bytes-4;
		*(long *)sizer = n_bytes;
		printf("Available Memory left in block: %lu bytes\n", new_size);
		long *old = *(long **)free_list; //Save head of list
		*(long **)look_through = NULL; //clear mem prev stored
		handler->free_list+=(n_bytes); //Increment free list by n_bytes and make a size header
		*(long *)handler->free_list = new_size;
		handler->free_list+=4; 
		*(long **)handler->free_list = old; //Store the next free location in new slot
		printf("Allocated Space at %p; Free List at %p\n", look_through, handler->free_list);
		return look_through;
	}else{ //middle or tail of list
		if(prev == NULL) return NULL;
		//printf("middle or tail of list\n");
		//Modify the size of list
		void *sizer = look_through-4; //modify the size of the list
		long new_size = *(long *)sizer - n_bytes-4;
		*(long *)sizer = n_bytes;
		printf("Available Memory left in block: %lu bytes\n", new_size);
		//Set up new free node (fragmented memory)
		void *modifier = look_through;
		long *old = *(long**)look_through;
		*(long **)look_through = NULL; //clear mem prev stored
		modifier += n_bytes;
		*(long *)modifier = new_size;
		modifier +=4;
		//Link previous data to new data and new data to prev.next
		*(long **)modifier = old; //curr.next = old pointer val
		*(long **)prev = (long *)modifier; //prev.next = curr
		
		printf("Allocated Space at %p; Free List at %p\n", look_through, handler->free_list);
		return look_through;
	}
	return NULL;
}

void *allocate_to_list_exact(info_p handler, void *free_list, void *look_through, long n_bytes, void *prev){
	//Linked list with pointer arithmetic
	printf("Allocating to free list (exact size)\n");
	//printf("free:%p; look_through: %p\n",free_list,look_through);
	//if(flag==1)printf("Allocating full memory!\n");
	if(look_through == free_list){ //head of list
		void *sizer = look_through-4; //modify the size of the list
		long new_size = *(long *)sizer - n_bytes;
		*(long *)sizer = n_bytes;
		printf("Available Memory left in block: %lu bytes\n",  new_size);
		long *old = *(long **)free_list; //Save next free location
		*(long **)look_through = NULL; //clear mem prev stored
		handler->free_list=old; //Head has been allocated with exact size, head = head->next
		printf("Allocated Space at %p; Free List at %p\n", look_through, handler->free_list);
		return look_through;
	}else{ //middle or tail of list
		//printf("middle or tail of list\n");
		void *sizer = look_through-4; //modify the size of the list
		long new_size = *(long *)sizer - n_bytes;
		*(long *)sizer = n_bytes;
		printf("Available Memory left in block: %lu bytes\n",  new_size);
		if(prev == NULL) return NULL;
		long *old = *(long **)look_through;
		*(long **)look_through = NULL; //clear mem prev stored
		//prev.next = curr.next
		*(void **)prev = old;
		printf("Allocated Space at %p; Free List at %p\n", look_through, handler->free_list);
		return look_through;
	}
	return NULL;
}

void free_list_free(int handle, void *region){
	info_p handler = &MemAllocs[handle];
	if(handler->free_list == NULL){
		handler->free_list = region;
		print_free_list(handler); //debug print
		return;
	}
	if(region < handler->free_list){
		//Freed memory belongs at the beginning of free list
		printf("Freeing Region: %p; Current Free List: %p\n", region, handler->free_list);
		*(long **)region = handler->free_list;
		handler->free_list = region;
		printf("New Free Pointer: %p\n", handler->free_list);
	}else{
		//Freed memory belongs inside the queue
		long *prev = NULL;
		long *curr = handler->free_list;
		while(curr != NULL && (long *)curr < (long *)region){ //traverse free list
			prev = curr;
			curr = *(long**)curr;
		}
		//Modify Free List (add in new free node)
		*(long **)prev = (long *)region;
		*(long **)region = (long *) curr;
	}
	
	coalesce_free_list(handler);
	print_free_list(handler); //debug print
}

void print_free_list(info_p handler){
	int i = 0;
	long *curr = handler->free_list;
	while(curr != NULL){
		printf("Free List %d: %p\n", i, curr);
		curr = *(long**)curr;
		i++;
	}
}

void coalesce_free_list(info_p handle){
	void *curr=handle->free_list;
	while(curr != NULL){
		long size = *(long *)(curr-4);
		//printf("Size of curr list: %lu\n", size);
		void *next = *(long **)curr;
		if(curr + size + 4 == next){
			*(long *)(curr-4) = size + *(long *)(next-4) + 4;
			*(long **)curr = *(long **)next;
			continue;
		}
		curr = *(long**)curr;
	}
	//printf("Free List: size: %lu @ %p\n", *(long *)(handle->free_list -4), handle->free_list);
}






