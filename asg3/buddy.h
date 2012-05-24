#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


extern const char zero;
extern const char one;
extern const char two;
extern const char four;
extern const char eight;
extern const char sixt;
extern const char thryt;
extern const char sixfour;
extern const char onetwoeght;

/*******************************************
Function: buddy alloc
This uses the buddy allocation algorithm to split and share
memory. This will use the bitmap to split up the memory
Returns: a void pointer to the memory of size n_bytes, or NULL if none available
*******************************************/
void *buddy_alloc(void *memptr, int overhead, long n_bytes, long mem_size, int page_size);


/*******************************************
Function: find_mem
This is the meat of the buddy allocation code. The design of the algorithm
is described in buddy_allocation_design.txt
Returns: a void pointer to the memory of size n_bytes, or NULL if none available
*******************************************/
void *find_mem(void *memptr, double num_bytes, int page_size, long alloc_size, 
               long mem_size, int* offset);


