#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//Struct Definition
struct info{
	void *memptr;
	long n_bytes;
	unsigned int flags;
	int parm1;
	int *parm2;
}MemAllocs[512];

typedef struct info * info_p;

//Macros for Allocation Type
#define BUDDY_ALLOC 0x1
#define SLAB_ALLOC 0x2
#define FREE1_ALLOC 0x4|0x00
#define FREE2_ALLOC 0x4|0x08
#define FREE3_ALLOC 0x4|0x10
#define FREE4_ALLOC 0x4|0x18
#define BYTE_SIZE 8

//Function Declarations

/*******************************************
Function: IsPowerOfTwo
Checks if the value is a power of two
Used for Buddy Allocation
Returns: 0 if false, non-zero if true
*******************************************/
int IsPowerOfTwo(long x);

/*******************************************
Function: OverHeadBytes
Calculates the number of bytes needed to maintain the bitmap needed
for Buddy Allocation. Uses the ceil() function to calculate the ceiling
of the number of chunks / BYTE_SIZE(8)
Used for Buddy Allocation
Returns: Number of bytes needed range: 1-X
*******************************************/
long OverHeadBytes(long chunks);

/*******************************************
Function: meminit
Initializes memory and takes up one
slot of MemAllocs to store memory
information. All information is stored
inside of the info structure
Return: Handle to the allocated memory
*******************************************/
int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2);

/*******************************************
Function: memalloc
Takes handle that specifies the allocator, and the number 
of bytes to allocate and returns them.
Return: A void pointer to the allocated memory
*******************************************/
void *memalloc(int handle, long n_bytes);
