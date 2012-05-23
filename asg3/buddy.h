#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define ZERO        0
#define ONE         1
#define TWO         2
#define FOUR        4
#define EIGHT       8
#define SIXT       16
#define THRYT      32
#define SIXFOUR    64
#define ONETWOEGHT 128

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
void *find_mem(void *memptr, long num_bytes, int page_size, long alloc_size, 
               long mem_size, int offset);


