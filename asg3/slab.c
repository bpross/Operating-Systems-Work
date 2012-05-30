#include "memory.h"
#include "slab.h"

void slab_free(info_p handle, void *region){
  printf("Freeing region: %p\n", region);
  if(handle->slab_full_list != NULL){
    //scan full list
    printf("Scanning full list\n");
    find_region(handle,handle->slab_full_list, region);
  }
  if(handle->slab_partial_list != NULL){
    printf("Scanning partial list\n");
    find_region(handle, handle->slab_partial_list, region);
  }
}

void find_region(info_p handle, void *regiontype, void *region){
    //scan partial list
    void *partial = regiontype;
    int regtype=0;
    if(regiontype == handle->slab_free_list){
      regtype = 1;
    }
    else if(regiontype == handle->slab_partial_list){
      regtype = 2;
    }
    else if(regiontype == handle->slab_full_list){
      regtype = 3;
    }

    while(partial != NULL){
      //printf("output message\n");
      //scan for region
      if((region >= partial) && (region < partial+handle->slabsize)){
	
	//printf("Found slab with the region\n");
	long chunksize = *(long *)(partial -8);
	int numchunks = (int)handle->slabsize / (int)chunksize;
	//printf("Chunk size of region is: %lu, numchunks: %d\n", chunksize, numchunks);
	void *prev = NULL;
	void *prevprev = NULL;
	void *curr = partial;
	//printf("chunk: %p\n", *(void **)(curr-4));
	//scan for region inside of partial
	while((curr < region) && curr != NULL){
	  prevprev = prev;
	  prev = curr;
	  curr = *(void **)curr;
	}
	
	//printf("chunk: %p\n", *(void **)(curr-4));
	//printf("prev: %p, curr: %p\n", prev, curr);
	if(prev == NULL){ // first node in partial list
	  
	  //search chunks for right chunk
	  void *chunkprev = NULL;
	  void *chunkcurr = *(void **)(curr-4);
	  while((chunkcurr < region) && chunkcurr != NULL){
	    chunkprev = chunkcurr;
	    chunkcurr = *(void **)chunkcurr;
	  }
	  if(chunkcurr == NULL){
	    //head = region
	    //printf("setting region to null\n");
	    *(void **)(curr-4) = region;
	    *(void **)region = NULL;
	  }
	  //printf("chunkcurr: %p\n", *(void**)(curr-4));
	  else if(chunkprev == NULL){
	    *(void **)chunkcurr = *(void**)(curr-4);
	    *(void **)(curr-4) = chunkcurr;
	  }else{
	    //printf("here\n");
	    *(void **)region = *(void **)chunkprev;
	    *(void **)chunkprev = region;
	  }
	  
	  int numnodes = 0;
	  void *chunktraverse = *(void **)(curr-4);
	  while(chunktraverse != NULL){
	    //printf("address is: %p, incrementing\n", chunktraverse);
	    numnodes++;
	    chunktraverse = *(void **)chunktraverse;
	  }
	  //printf("numnodes: %d\n", numnodes);

	  if(numnodes == numchunks){
	    printf("Partial/Full list to Free List\n");
	    if(prev == NULL){
	      void *freeprev = NULL;
	      void *freecurr = handle->slab_free_list;
	      while(freecurr < curr){
		freeprev = freecurr;
		freecurr = *(void **)freecurr;
	      }
	      
	      if(freeprev == NULL){ //insert to head of free list
		//Insert to front of free list and remove from partial/full list
		void *saver = *(void **)(regiontype-12);
		*(void **)(regiontype-12) = handle->slab_free_list;
		handle->slab_free_list = regiontype;
		//printf("saver: %p\n", saver);
		switch(regtype){
		case 1:
		  handle->slab_free_list = saver;
		  break;
		case 2:
		  handle->slab_partial_list = saver;
		  break;
		case 3:
		  handle->slab_full_list = saver;
		  break;
		}
	      }else{
		//Insert to middle/end of free list and remove from partial/full list
		void *saver = *(void **)(regiontype-12);
		//region.next = prev.next
		*(void **)(regiontype-12) = *(void **)(freeprev-12);
		//prev.next = region
		*(void **)(freeprev-12) = regiontype;
		//handle->slab_partial_list = saver;
		switch(regtype){
		case 1:
		  handle->slab_free_list = saver;
		  break;
		case 2:
		  handle->slab_partial_list = saver;
		  break;
		case 3:
		  //printf("full list: %p\n", saver);
		  handle->slab_full_list = saver;
		  break;
		}
	      }
	    }
	  }

	}else{ //middle node in partial list
	  //search chunks for right chunk
	  //printf("prev chunk: %p\n", *(void **)(prev-4));
	  if(*(void **)(prev-4) == NULL){
	    //head = region
	    //printf("here\n");
	    *(void **)(prev-4) = region;
	    *(void **)region = NULL;
	  }

	  else{
	    void *chunkprev = NULL;
	    void *chunkcurr = *(void **)(prev-4);
	    while((chunkcurr < region) && chunkcurr != NULL){
	      chunkprev = chunkcurr;
	      chunkcurr = *(void **)chunkcurr;
	    }
	   
	    if(chunkprev == NULL){
	      
	      *(void **)chunkcurr = *(void**)(prev-4);
	      *(void **)(prev-4) = chunkcurr;
	    }else{
	      
	      *(void **)region = *(void **)chunkprev;
	      *(void **)chunkprev = region;
	    }
	  }

	  int numnodes = 0;
	  void *chunktraverse = *(void **)(prev-4);
	  while(chunktraverse != NULL){
	    numnodes++;
	    chunktraverse = *(void **)chunktraverse;
	  }
	  //printf("numnodes: %d\n", numnodes);
	  if(numnodes == numchunks){
	    //If number of chunks is max, must swap to free list
	    printf("Partial/Full list to Free List\n");
	    if(prev == NULL){
	      //Never Occurs
	    }else{ // in middle of list
	      void *freeprev = NULL;
	      void *freecurr = handle->slab_free_list;
	      while(freecurr < curr){
		freeprev = freecurr;
		freecurr = *(void **)freecurr;
	      }
	      //printf("prev: %p, freeprev: %p, freecurr %p\n", prevprev, freeprev, freecurr);
	      if(freeprev == NULL){
		//Insert to beginning of free list and remove from partial/full list
		//save the next val
		void *saver = *(void **)(prev-12);
		//prev.next = free list
		*(void **)(prev-12) = handle->slab_free_list;
		//free list.head = prev
		handle->slab_free_list = prev;
		//*(void **)(prev-12) = saver;
		if(prevprev == NULL){
		  handle->slab_full_list = NULL;
		}
	      }else{
		//insert into middle/end of free list and remove from partial/full list
		//link prevprev.next = curr
		*(void **)(prevprev-12) = curr;
		*(void **)(prev-12) = freecurr;
		*(void **)(freeprev-12) = prev;
	      }
	    }
	  }
	}

	return;
      }
      partial = *(void **)(partial -12);
    }
  
}


void *slab_allocation(info_p handle, long n_bytes){
  //printf("hi\n");
  int i=0;
  void *returnptr = NULL;
  while(n_bytes > handle->parm2[i]){
    i++;
    if(handle->parm2[i] == 0){
      printf("Could not find an available chunk\n");
    }
  }
  int chunksize = handle->parm2[i];
  //printf("Designated chunk size: %d\n", chunksize);

  if(handle->slab_partial_list == NULL){
    //printf("Allocating from free list\n");
    returnptr = allocate_free_list(handle, chunksize);
  }else{
    returnptr = allocate_partial_list(handle, chunksize);
  }
  if(returnptr != NULL)
    printf("Slab Allocation: Allocated memory at: %p\n", returnptr);
  return returnptr;
}

void *allocate_partial_list(info_p handle, long n_bytes){
  //printf("allocating from partial list\n");
  void *partial = handle->slab_partial_list;
  void *returnptr=NULL;
  while(*(void**)(partial-12) != NULL){
    if(*(long *)(partial-8) == n_bytes) break;
    partial = *(void **)(partial-12);
  }
  if(partial == NULL){ //no partial has correct chunk size
    //allocate from free list
    returnptr = allocate_free_list(handle, n_bytes);
  }else{ //found a partial that can fit
    printf("Allocating from partial list\n");
    //returnptr is next chunk
    returnptr = *(void **)(partial-4);
    /*printf("chunk: %p; next: %p\n", 
	   *(void **)(partial-4),
	   *(void **)*(void **)(partial -4));*/
    //chunk = chunk.next
    *(void **)(partial-4) = *(void **)*(void **)(partial -4);
    //if no more chunks, move to full list
    if(*(void**)(partial-4)==NULL){
      printf("Moving partial to full list\n");
      //if full list is empty, head = partial
      if(handle->slab_full_list == NULL){
	handle->slab_full_list = partial;
	*(void **)(handle->slab_full_list-12) = NULL;
      }else{ //else insert to middle or end
	insert_to_list(handle, handle->slab_full_list, partial);
      }
      //increment partial to partial.next
      handle->slab_partial_list = *(void **)(partial-12);
    }
  }

  return returnptr;
}


void *allocate_free_list(info_p handle, long n_bytes){
  if(handle->slab_free_list == NULL){
    printf("No more free space\n");
    return NULL;
  }
  printf("Allocating from free list\n");
  //Update Chunk Size
  printf("Chunk size: %lu\n", n_bytes);
  *(long *)(handle->slab_free_list-8) = n_bytes;
  set_up_chunk_ptrs(handle, n_bytes);

  //Debug Print
  /*void *chunkptr = handle->slab_free_list;
  printf("slab size: %d\n", handle->slabsize);
  while(chunkptr != NULL){
    printf("Chunkptr: %p\n", chunkptr);
    chunkptr = *(void **)chunkptr;
    }*/

  //returnptr = free chunk
  void *returnptr = handle->slab_free_list;

  //chunkptr = head of chunklist
  void *chunkptr = *(void **)(handle->slab_free_list-4);
  //printf("chunkptr: %p; next: %p\n", chunkptr, *(void **)chunkptr);

  //head of chunk list = head.next
  *(void **)(handle->slab_free_list-4) = *(void **)chunkptr;

  //if head is null, move to full list
  if(*(void **)(handle->slab_free_list-4) == NULL){
    printf("Moving to full list\n");
    if(handle->slab_full_list == NULL){ //full is is currently null
      handle->slab_full_list = handle->slab_free_list;
      *(void **)(handle->slab_full_list-12) = NULL;
    }else{ //full list not null
      insert_to_list(handle, handle->slab_full_list, handle->slab_free_list);
    }
    handle->slab_free_list = *(void **)handle->slab_free_list;
  }else{ //Free list is now a partial list
    if(handle->slab_partial_list == NULL){
      handle->slab_partial_list = handle->slab_free_list;
      handle->slab_free_list = *(void **)(handle->slab_free_list-12);
      *(void **)(handle->slab_partial_list-12) = NULL;
    }else{
      insert_to_list(handle, handle->slab_partial_list, handle->slab_free_list);
      handle->slab_free_list = *(void **)(handle->slab_free_list-12);
    }
    //handle->slab_free_list = *(void **)(handle->slab_free_list-12);
  } 

  return returnptr;
}


//TEST TO SEE THIS WORKS
void insert_to_list(info_p handle, void *headptr, void *insert){
  void *prev = NULL;
  while(headptr < insert){
    headptr = *(void **)headptr;
    if(headptr == NULL) break;
  }
  int listtype = 0;
  if(handle->slab_free_list == insert) listtype = 1;
  else if(handle->slab_partial_list == insert) listtype = 2;
  else if(handle->slab_full_list == insert) listtype = 3;

  if(prev == NULL){ //insert to head of list
    
    void *saver = *(void **)(insert-12);
    *(void**)(insert-12) = headptr;
    handle->slab_full_list = insert;
    switch(listtype){
    case 1:
      handle->slab_free_list = saver;
      break;
    case 2:
      handle->slab_partial_list = saver;
      break;
    case 3:
      handle->slab_full_list = saver;
      break;
    }
  }else{ //insert to middle or end
    void *saver = *(void **)(insert-12);
    *(void **)(insert-12) = *(void **)(prev-12);
    *(void **)(prev-12) = insert;
    switch(listtype){
    case 1:
      handle->slab_free_list = saver;
      break;
    case 2:
      handle->slab_partial_list = saver;
      break;
    case 3:
      handle->slab_full_list = saver;
      break;
    }
  }
}

void set_up_chunk_ptrs(info_p handle, long n_bytes){
  void *chunklist = handle->slab_free_list;
  int chunksize = *(long*)(chunklist-8);
  *(void **)(chunklist-4) = chunklist;
  //printf("chunksize: %d\n", chunksize);
  //printf("end: %p\n", handle->slab_free_list + handle->slabsize);
  while(chunklist < handle->slab_free_list + handle->slabsize){
    if(chunklist + chunksize >= handle->slab_free_list + handle->slabsize){
      *(void **)chunklist = NULL;
      return;
    }
    *(void **)chunklist = chunklist + chunksize;
    chunklist = *(void **)chunklist;
  }
}

void format_slab_list(void *memptr, int memsize, int slabsize){
  //printf("memptr: %p, memsize: %d, slabsize: %d\n", memptr, memsize, slabsize);
  int num_oh_items = 3;
  int overhead = 4*num_oh_items;
  void *startptr = memptr;

  //Insert first ptr to first slab
  *(void **)memptr = (void *)memptr+slabsize+2*overhead;
  //printf("next: %p\n", *(void **)memptr);

  //Inserting rest of ptrs
  //printf("%p comp %p, memsize: %d\n", *(void **)memptr, ((void*)memptr+memsize), memsize);
  while( (memptr < ((void*)startptr+memsize)) ){
    memptr = *(void **)memptr;
    memptr = memptr-overhead;
    if(memptr + slabsize+2*overhead > startptr + memsize){
      *(void **)memptr = NULL;
      return;
    }
    //*(void **)memptr = memptr + slabsize + 2*overhead;
    void *newptr = memptr + slabsize + 2*overhead;
    //printf("newptr: %p\n", newptr);
    *(void **)memptr = newptr;
  }
  
}

void print_slab_list(void *memptr){
  printf("Printing Slab List\n");
  if(memptr==NULL){
    printf("Nothing in slab list\n");
    return;
  }
  int ohsize = 3;
  int overhead = 4*ohsize;
  memptr = memptr-overhead;
  while(*(void **)memptr != NULL){
    int diff = *(void **)memptr - memptr - overhead;
    printf("Curr Addr: %p, Next Addr: %p\n", memptr, *(void **)memptr);
    memptr = *(void **)memptr - overhead;
  }
  printf("Curr Addr: %p, Next Addr: %p\n", memptr, *(void **)memptr);

}
